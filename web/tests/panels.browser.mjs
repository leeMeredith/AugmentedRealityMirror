// Session-overlay focus, camera-only sidebar and compact scoring; simulated camera only.
import assert from "node:assert/strict";
const {chromium}=await import(process.env.ARM_PLAYWRIGHT_MODULE||"playwright");
const browser=await chromium.launch({headless:true,
 ...(process.env.ARM_CHROME_EXECUTABLE?{executablePath:process.env.ARM_CHROME_EXECUTABLE}:{}),
 args:["--use-fake-device-for-media-stream","--use-fake-ui-for-media-stream"],
});
try {
 const page=await browser.newPage({viewport:{width:1440,height:1000}}),errors=[];
 page.on("pageerror",e=>errors.push(e.message));
 const assets=[];page.on("request",r=>{if(["script","stylesheet"].includes(r.resourceType()))assets.push(new URL(r.url()));});
 await page.goto(process.env.ARM_TEST_URL||"http://localhost:8001");
 assert.equal(await page.locator("#build-version").textContent(),"Browser build 12");
 assert.ok(assets.length>0&&assets.every(url=>url.searchParams.get("v")==="12"));
 assert.equal(await page.locator("#session-summary").isHidden(),true);
 assert.equal(await page.locator("#study-session").isHidden(),true);
 await page.locator("#start-camera").click();
 await page.waitForFunction(()=>document.querySelector("#camera-state").textContent==="Camera on");
 const cameraLayout=async label=>{
  await page.locator("#view-controls").evaluate(el=>{el.scrollTop=0;});
  const before=await page.locator("#mirror-canvas").boundingBox();
  const heading=await page.locator("#camera-panel-title").boundingBox();
  const sidebar=await page.locator("#controls").boundingBox();
  assert.ok(sidebar.x>=before.x+before.width,`${label}: camera controls on right`);
  await page.locator("#view-controls").evaluate(el=>{el.scrollTop=el.scrollHeight;});
  assert.ok(await page.locator("#view-controls").evaluate(el=>el.scrollTop>0));
  assert.deepEqual(await page.locator("#mirror-canvas").boundingBox(),before);
  assert.deepEqual(await page.locator("#camera-panel-title").boundingBox(),heading);
  assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),true);
  await page.locator("#view-controls").evaluate(el=>{el.scrollTop=0;});
 };
 for(const [width,height] of [[1440,1000],[390,600],[780,400]]) {
  await page.setViewportSize({width,height});await cameraLayout(`${width}`);
  await page.locator("#open-session").click();
  assert.equal(await page.locator("#loss-right-arm").isVisible(),true);
  assert.equal(await page.locator("#loss-left-leg").isVisible(),true);
  // Modal traps tab focus and keeps its own content scroll, header and actions fixed.
  await page.keyboard.press("Shift+Tab");
  assert.equal(await page.evaluate(()=>document.querySelector("#study-session").contains(document.activeElement)),true);
  const heading=await page.locator("#session-title").boundingBox();
  const actions=await page.locator(".session-actions").boundingBox();
  await page.locator("#session-content").evaluate(el=>{el.scrollTop=el.scrollHeight;});
  assert.deepEqual(await page.locator("#session-title").boundingBox(),heading);
  assert.deepEqual(await page.locator(".session-actions").boundingBox(),actions);
  assert.ok(actions.y+actions.height<=height);
  await page.keyboard.press("Escape");
  assert.equal(await page.locator("#study-session").isHidden(),true);
  assert.equal(await page.locator("#open-session").evaluate(el=>el===document.activeElement),true);
  await page.locator("#open-session").click();
  if(process.env.ARM_SCREENSHOT_DIR)await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/overlay-${width}.png`});
  await page.locator("#close-session").click();
  if(process.env.ARM_SCREENSHOT_DIR)await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/camera-${width}.png`});
 }
 await page.setViewportSize({width:1440,height:1000});
 await page.locator("#open-session").click();await page.locator("#participant-name").fill("Example Person");
 await page.locator("#loss-right-arm").check();await page.locator("#loss-right-arm-level").fill("below elbow");
 await page.locator("[data-score='5']").click();await page.locator("#start-session").click();
 assert.equal(await page.locator("#study-session").isHidden(),true);
 assert.equal(await page.locator("#open-session").textContent(),"Current session");
 await page.locator("#record-pain-view").click();await page.locator("#quick-pain-scores").getByRole("button",{name:"Pain score 0",exact:true}).click();
 assert.equal(await page.locator("#quick-pain-panel").isHidden(),true);
 assert.match(await page.locator("#session-summary").textContent(),/last pain 0\/10/);
 await page.locator("#fullscreen-view").click();await page.waitForFunction(()=>!!document.fullscreenElement);
 assert.ok((await page.locator("#fullscreen-view").boundingBox()).height<=36);
 await page.locator("#open-session").click();
 assert.match(await page.locator("#limb-summary").textContent(),/right arm \(below elbow\)/);
 // Native modal Escape closes the session without also leaving camera fullscreen.
 await page.keyboard.press("Escape");
 assert.equal(await page.locator("#study-session").isHidden(),true);
 assert.equal(await page.evaluate(()=>!!document.fullscreenElement),true);
 await page.locator("#record-pain-view").click();await page.keyboard.press("Escape");
 assert.equal(await page.locator("#quick-pain-panel").isHidden(),true);
 assert.equal(await page.evaluate(()=>!!document.fullscreenElement),true);
 await page.locator("#end-session-view").click();
 assert.equal(await page.locator("#study-session").isVisible(),true);
 assert.equal(await page.locator("#pain-phase").textContent(),"Pain at end of use");
 const download=page.waitForEvent("download");await page.locator("[data-score='2']").click();await download;
 await page.locator("#new-session").click();
 assert.equal(await page.locator("#participant-name").inputValue(),"Example Person");
 assert.equal(await page.locator("#participant-name").isEnabled(),true);
 assert.equal(await page.locator("#loss-right-arm").isChecked(),true);
 await page.locator("#close-session").click();await page.keyboard.press("Escape");
 await page.waitForFunction(()=>!document.fullscreenElement);
 // Backdrop dismissal keeps entered setup information.
 await page.locator("#open-session").click();await page.mouse.click(5,5);
 assert.equal(await page.locator("#study-session").isHidden(),true);
 await page.locator("#open-session").click();assert.equal(await page.locator("#loss-right-arm").isChecked(),true);
 assert.deepEqual(errors,[]);
 console.log(`PASS Camera settings on right, modal focus/scroll/dismissal, visible limb details, preserved forms, quick scoring, compact toolbar and fullscreen modal at desktop/narrow/short sizes. Chrome ${browser.version()}`);
}finally{await browser.close();}
