// Independent control-box scrolling and pinned video/actions; synthetic camera only.
import assert from "node:assert/strict";
const {chromium}=await import(process.env.ARM_PLAYWRIGHT_MODULE||"playwright");
const browser=await chromium.launch({headless:true,
 ...(process.env.ARM_CHROME_EXECUTABLE?{executablePath:process.env.ARM_CHROME_EXECUTABLE}:{}),
 args:["--use-fake-device-for-media-stream","--use-fake-ui-for-media-stream"],
});
try {
 const page=await browser.newPage({viewport:{width:1440,height:1000}}), errors=[];
 page.on("pageerror",e=>errors.push(e.message));
 const assets=[];page.on("request",r=>{if(["script","stylesheet"].includes(r.resourceType()))assets.push(new URL(r.url()));});
 await page.goto(process.env.ARM_TEST_URL||"http://localhost:8001");
 assert.equal(await page.locator("#build-version").textContent(),"Browser build 11");
 assert.equal(await page.getByRole("tab").count(),0);
 assert.ok(assets.length>0&&assets.every(url=>url.searchParams.get("v")==="11"));
 await page.locator("#start-camera").click();
 await page.waitForFunction(()=>document.querySelector("#camera-state").textContent==="Camera on");
 const check=async label=>{
  const before=await page.evaluate(()=>{
   const box=id=>{const r=document.getElementById(id).getBoundingClientRect();return {x:r.x,y:r.y,width:r.width,height:r.height};};
   document.querySelector("#view-controls").scrollTop=0;
   document.querySelector("#session-content").scrollTop=0;
   return {video:box("mirror-canvas"),cameraHeading:box("camera-panel-title"),sessionHeading:box("session-title"),actions:document.querySelector(".control-box-actions").getBoundingClientRect().toJSON()};
  });
  await page.locator("#view-controls").evaluate(el=>{el.scrollTop=el.scrollHeight;});
  assert.equal(await page.locator("#session-content").evaluate(el=>el.scrollTop),0,label);
  assert.ok(await page.locator("#view-controls").evaluate(el=>el.scrollTop>0),label);
  const cameraScroll=await page.locator("#view-controls").evaluate(el=>el.scrollTop);
  await page.locator("#session-content").evaluate(el=>{el.scrollTop=el.scrollHeight;});
  assert.ok(await page.locator("#session-content").evaluate(el=>el.scrollTop>0),label);
  assert.equal(await page.locator("#view-controls").evaluate(el=>el.scrollTop),cameraScroll,label);
  const result=await page.evaluate(()=>{
   const box=id=>{const r=document.getElementById(id).getBoundingClientRect();return {x:r.x,y:r.y,width:r.width,height:r.height};};
   const a=document.querySelector("#camera-controls-box").getBoundingClientRect(),b=document.querySelector("#study-session").getBoundingClientRect();
   const c=document.querySelector(".control-box-actions").getBoundingClientRect(),video=document.querySelector("#mirror-canvas").getBoundingClientRect();
   return {video:box("mirror-canvas"),cameraHeading:box("camera-panel-title"),sessionHeading:box("session-title"),
    separate:a.bottom<=b.top||a.right<=b.left, cameraScrollable:document.querySelector("#view-controls").clientHeight>0,
    sessionScrollable:document.querySelector("#session-content").clientHeight>0,
    actionsFit:c.top>=b.top&&c.bottom<=b.bottom&&c.bottom<=innerHeight,
    videoFits:video.top>=0&&video.bottom<=innerHeight&&video.width>0&&video.height>0,
    pageFits:document.documentElement.scrollWidth<=innerWidth};
  });
  assert.deepEqual(result.video,before.video,label);assert.deepEqual(result.cameraHeading,before.cameraHeading,label);assert.deepEqual(result.sessionHeading,before.sessionHeading,label);
  for(const key of ["separate","cameraScrollable","sessionScrollable","actionsFit","videoFits","pageFits"]) assert.equal(result[key],true,`${label}: ${key}`);
  await page.locator("#view-controls").evaluate(el=>{el.scrollTop=0;});
  await page.locator("#session-content").evaluate(el=>{el.scrollTop=0;});
  if(process.env.ARM_SCREENSHOT_DIR) await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/panels-${label}.png`});
 };
 await check("desktop");
 const heights=await page.evaluate(()=>[document.querySelector("#camera-controls-box").clientHeight,document.querySelector("#study-session").clientHeight]);
 assert.ok(heights[1]>heights[0]);
 await page.locator("[data-score='5']").click();await page.locator("#start-session").click();
 await check("active");
 await page.locator("#end-session").click();
 const download=page.waitForEvent("download");await page.keyboard.press("3");await download;
 await page.locator("#fullscreen-view").click();await page.waitForFunction(()=>!!document.fullscreenElement);
 await page.locator("#toggle-controls").click();await check("fullscreen");
 await page.keyboard.press("Escape");await page.waitForFunction(()=>!document.fullscreenElement);
 for(const [width,height] of [[390,600],[780,400]]){await page.setViewportSize({width,height});await check(String(width));}
 assert.deepEqual(errors,[]);
 console.log(`PASS Both boxes visible, independent scrolling, fixed headings/video, Session space and pinned actions at desktop/fullscreen/narrow/short sizes. Chrome ${browser.version()}`);
}finally{await browser.close();}
