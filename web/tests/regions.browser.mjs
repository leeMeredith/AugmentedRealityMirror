// Optional headless integration checks. Only synthetic images/camera are used.
import assert from "node:assert/strict";
import { readFile } from "node:fs/promises";
const { chromium } = await import(process.env.ARM_PLAYWRIGHT_MODULE || "playwright");
const browser = await chromium.launch({ headless: true,
  ...(process.env.ARM_CHROME_EXECUTABLE ? { executablePath: process.env.ARM_CHROME_EXECUTABLE } : {}),
  args: ["--use-fake-device-for-media-stream", "--use-fake-ui-for-media-stream"],
});
try {
  const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } });
  const errors = [];
  page.on("pageerror", error => errors.push(error.message));
  await page.goto(process.env.ARM_TEST_URL || "http://localhost:8001");
  assert.equal(await page.locator("#build-version").textContent(), "Browser build 8");
  const pixels = await page.evaluate(async () => {
    const { drawRegions, defaultRegions, REGION_IDS } = await import("./src/mirrorRegions.js");
    const source = document.createElement("canvas"), target = document.createElement("canvas");
    const input = source.getContext("2d"), output = target.getContext("2d");
    let cases = 0;
    for (const [width,height] of [[80,60],[81,61]]) {
      source.width = target.width = width; source.height = target.height = height;
      const image = input.createImageData(width,height);
      for(let y=0;y<height;y++) for(let x=0;x<width;x++) image.data.set([30+x*2,40+y*2,123,255],(y*width+x)*4);
      input.putImageData(image,0,0);
      for (const cx of [10,20,33,50,71,80,90]) for (const cy of [10,20,33,50,71,80,90]) {
        const center = { x: cx, y: cy };
        const splitX=Math.round(width*center.x/100),splitY=Math.round(height*center.y/100);
        for(const mirrored of [false,true]) for(let mask=0;mask<16;mask++) {
          const regions=defaultRegions();
          REGION_IDS.forEach((id,i)=>{regions[id].mirrorX=!!(mask&(1<<i));});
          output.clearRect(0,0,width,height);
          drawRegions(output,source,width,height,mirrored,regions,center);
          const actual=output.getImageData(0,0,width,height).data;
          for(let y=0;y<height;y++) for(let x=0;x<width;x++) {
            const i=(y*width+x)*4, region=(y<splitY?0:2)+(x<splitX?0:1);
            if(actual[i+3]!==255 || actual[i+2]!==123) return {error:"gap",center,mirrored,mask,x,y,pixel:[...actual.slice(i,i+4)]};
            // Every pixel must remain at its original full-frame coordinate, or its
            // exact horizontal reflection. This catches zoom and translation as well as gaps.
            const flipped=mirrored!==regions[REGION_IDS[region]].mirrorX;
            const expected=[30+(flipped?width-1-x:x)*2,40+y*2,123,255];
            if(expected.some((v,c)=>v!==actual[i+c])) return {error:"image moved or resized",center,mirrored,mask,x,y};
          }
          cases++;
        }
      }
    }
    return {cases};
  });
  assert.deepEqual(pixels,{cases:3136});
  console.log("PASS 3136 pixel cases: fixed full-image scale/position, no gaps, independent choices, both camera orientations and odd dimensions");
  assert.equal(await page.locator("#region-mirror-y").count(),0);
  await page.locator("#start-camera").click();
  await page.waitForFunction(()=>document.querySelector("#camera-state").textContent==="Camera on");
  await page.evaluate(()=>{window.originalTrack=document.querySelector("#camera-video").srcObject.getVideoTracks()[0];});
  await page.locator("#region-mirror-x").check();
  await page.locator("#center-x").fill("32");
  await page.locator("#center-y").fill("68");
  await page.locator("[data-region=bottomRight]").click();
  assert.equal(await page.locator("#center-x").inputValue(),"32");
  assert.equal(await page.locator("#center-y").inputValue(),"68");
  assert.equal(await page.locator("#region-mirror-x").isChecked(),false);
  await page.locator("#region-mirror-x").check();
  await page.locator("#setup-name").fill("Example chair");
  const downloadPromise=page.waitForEvent("download");
  await page.locator("#save-setup").click();
  const download=await downloadPromise;
  const saved=JSON.parse(await readFile(await download.path(),"utf8"));
  assert.match(download.suggestedFilename(),/^Example_chair_setup_/);
  assert.equal(saved.schemaVersion,3);
  assert.deepEqual(saved.view.center,{x:32,y:68});
  assert.deepEqual(saved.view.regions.topLeft,{mirrorX:true});
  assert.deepEqual(saved.view.regions.bottomRight,{mirrorX:true});
  await page.locator("#reset-alignment").click();
  assert.equal(await page.locator("#center-x").inputValue(),"50");
  assert.equal(await page.locator("#center-y").inputValue(),"50");
  assert.equal(await page.locator("#region-mirror-x").isChecked(),true);
  const load=async data=>{
    await page.locator("#setup-status").evaluate(el=>{el.textContent="";});
    await page.locator("#setup-file").setInputFiles({name:"setup.json",mimeType:"application/json",buffer:Buffer.from(JSON.stringify(data))});
    await page.waitForFunction(()=>document.querySelector("#setup-status").textContent!=="");
  };
  await load(saved);
  assert.equal(await page.locator("#center-x").inputValue(),"32");
  const bad=structuredClone(saved);bad.view.center.y=200;
  await load(bad);
  assert.match(await page.locator("#setup-status").textContent(),/invalid alignment/);
  assert.equal(await page.locator("#center-y").inputValue(),"68");
  const old=structuredClone(saved);old.schemaVersion=2;delete old.view.center;
  for(const region of Object.values(old.view.regions)) Object.assign(region,{mirrorY:true,offsetX:12,offsetY:-20});
  await load(old);
  assert.match(await page.locator("#setup-status").textContent(),/Top\/bottom flips and individual offsets removed/);
  assert.equal(await page.locator("#center-y").inputValue(),"50");
  assert.equal(await page.locator("#region-mirror-x").isChecked(),true);
  console.log("PASS Shared sliders, independent toggles, global reset, schema 3 round trip, invalid-file preservation and explicit legacy migration");

  async function checkLayout(label) {
    await page.locator("#toggle-controls").focus();
    await page.locator("#controls").evaluate(el=>{el.scrollTop=0;});
    const before=await page.locator("#mirror-canvas").boundingBox();
    await page.locator("#save-setup").focus();
    const result=await page.evaluate(()=>{
      const panel=document.querySelector("#controls"),v=document.querySelector("#mirror-canvas").getBoundingClientRect(),c=panel.getBoundingClientRect();
      return {scrolled:panel.scrollTop>0,visible:v.width>0&&v.height>0&&v.top>=0&&v.bottom<=innerHeight&&v.left>=0&&v.right<=innerWidth,
        separate:c.left>=v.right || c.top>=v.bottom,overflow:document.documentElement.scrollWidth>innerWidth};
    });
    assert.deepEqual(result,{scrolled:true,visible:true,separate:true,overflow:false},label);
    assert.deepEqual(await page.locator("#mirror-canvas").boundingBox(),before,label);
    if(process.env.ARM_SCREENSHOT_DIR) await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/${label}.png`});
  }
  await checkLayout("desktop");
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(()=>!!document.fullscreenElement);
  await page.locator("#toggle-controls").click();
  await checkLayout("fullscreen");
  await page.keyboard.press("Escape");
  await page.waitForFunction(()=>!document.fullscreenElement);
  for(const [width,height] of [[390,600],[780,400]]) {
    await page.setViewportSize({width,height});
    await checkLayout(`small-${width}`);
  }
  await page.setViewportSize({width:390,height:600});
  await page.evaluate(()=>Object.defineProperty(document,"fullscreenEnabled",{value:false}));
  await page.locator("#fullscreen-view").click();
  await page.locator("#toggle-controls").click();
  await checkLayout("small-expanded");
  await page.locator("#center-y").focus();await page.keyboard.press("ArrowRight");
  assert.equal(await page.locator("#center-y").inputValue(),"51");
  assert.equal(await page.evaluate(()=>window.originalTrack.readyState),"live");
  await page.locator("#stop-view").click();
  assert.equal(await page.evaluate(()=>window.originalTrack.readyState),"ended");
  assert.deepEqual(errors,[]);
  console.log(`PASS Stationary visible video and separate scrolling controls at desktop, fullscreen, narrow and short sizes; keyboard and camera stop. Chrome ${browser.version()}`);
} finally {await browser.close();}
