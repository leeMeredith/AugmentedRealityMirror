// Isolated synthetic-camera checks for the single capture shared between two windows.
import assert from "node:assert/strict";
import {readFile} from "node:fs/promises";
const {chromium}=await import(process.env.ARM_PLAYWRIGHT_MODULE||"playwright");
const browser=await chromium.launch({headless:true,
 ...(process.env.ARM_CHROME_EXECUTABLE?{executablePath:process.env.ARM_CHROME_EXECUTABLE}:{}),
 args:["--use-fake-device-for-media-stream","--use-fake-ui-for-media-stream"],
});
const base=process.env.ARM_TEST_URL||"http://localhost:8001";
try {
 const context=await browser.newContext({viewport:{width:1360,height:900}});
 await context.addInitScript(()=>{
  window.captureCalls=0;
  if (!navigator.mediaDevices?.getUserMedia) return;
  const original=navigator.mediaDevices.getUserMedia.bind(navigator.mediaDevices);
  navigator.mediaDevices.getUserMedia=(...args)=>{window.captureCalls++;return original(...args);};
 });
 const page=await context.newPage(),errors=[];
 context.on("page",p=>p.on("pageerror",e=>errors.push(e.message)));
 page.on("pageerror",e=>errors.push(e.message));
 await page.goto(base);
 assert.equal(await page.locator("#build-version").textContent(),"Browser build 12");
 assert.equal(await page.locator("#camera-controls-box").isVisible(),true);
 assert.equal(await page.locator("#study-session").isVisible(),false);
 await page.locator("#open-session").click();
 await page.locator("#participant-name").fill("Example Person");
 await page.locator("#close-session").click();
 await page.locator("#start-camera").click();
 await page.waitForFunction(()=>document.querySelector("#camera-state").textContent==="Camera on");
 await page.evaluate(()=>{
  window.originalTrack=document.querySelector("#camera-video").srcObject.getVideoTracks()[0];
  window.originalCanvas=document.querySelector("#mirror-canvas");
 });

 await page.locator("#open-session").click();
 await page.locator("[data-score='6']").click();await page.locator("#start-session").click();
 const open=async()=>{
  const pending=page.waitForEvent("popup");await page.locator("#open-camera-window").click();
  const child=await pending;await child.locator("#mirror-canvas").waitFor({state:"visible"});return child;
 };
 let child=await open();
 assert.equal(await page.locator("#mirror-canvas").count(),0);
 assert.equal(await child.evaluate(()=>document.querySelector("#mirror-canvas")===opener.originalCanvas),true);
 assert.equal(await page.evaluate(()=>captureCalls),1);
 assert.equal(await child.evaluate(()=>captureCalls),0);
 assert.equal(await page.evaluate(()=>originalTrack.readyState),"live");
 assert.equal(await page.locator("#study-session").isVisible(),false);
 // Render continues with the controller tab in the background.
 await child.bringToFront();
 await child.evaluate(()=>{window.firstFrame=document.querySelector("#mirror-canvas").toDataURL();});
 await child.waitForFunction(()=>document.querySelector("#mirror-canvas").toDataURL()!==window.firstFrame);
 assert.equal(await child.locator("#camera-window-status").isHidden(),true);
 assert.equal(await child.locator("#camera-window-summary").textContent().then(t=>t.includes("Optional pain")),false);
 assert.ok((await child.locator("#camera-window-fullscreen").boundingBox()).height<=36);
 await child.locator("#camera-window-toggle").click();
 assert.equal(await child.locator("#camera-window-stop").isHidden(),true);
 await child.locator("#camera-window-toggle").click();
 await child.locator("#camera-window-score").click();
 await child.getByRole("button",{name:"Pain score 4",exact:true}).click();
 assert.equal(await child.locator("#camera-quick-panel").isHidden(),true);
 await child.keyboard.press("0");
 await page.waitForFunction(()=>document.querySelector("#session-summary").textContent.includes("10/10"));
 await child.locator("#camera-window-fullscreen").click();
 await child.waitForFunction(()=>!!document.fullscreenElement);
 await child.keyboard.press("Escape");await child.waitForFunction(()=>!document.fullscreenElement);
 if(process.env.ARM_SCREENSHOT_DIR) await child.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/camera-window.png`});
 await child.close();
 await page.locator("#mirror-canvas").waitFor({state:"visible"});
 assert.equal(await page.locator("#pain-history li").count(),3);
 assert.match(await page.locator("#session-summary").textContent(),/Session active/);
 child=await open();

 const download=async action=>{
  const pending=page.waitForEvent("download");await action();const d=await pending;
  return {name:d.suggestedFilename(),record:JSON.parse(await readFile(await d.path(),"utf8"))};
 };
 await child.locator("#camera-window-end").click();
 const session=await download(()=>child.keyboard.press("3"));
 assert.equal(session.record.participantName,"Example Person");
 assert.match(session.name,/^Example_Person_/);
 assert.deepEqual(session.record.painEntries.map(e=>e.painScore),[6,4,10,3]);

 const setup=await download(()=>page.locator("#save-setup").click());
 assert.equal(setup.record.schemaVersion,4);
 assert.equal(setup.record.participantName,session.record.participantName);
 assert.match(setup.name,/^Example_Person_.*_setup\.json$/);
 console.log("PASS One stream/canvas, live background-controller rendering, scoring/end from camera window, popup fullscreen and matching names in both JSON files");
 const load=async data=>{
  await page.locator("#setup-status").evaluate(el=>{el.textContent="";});
  await page.locator("#setup-file").setInputFiles({name:"setup.json",mimeType:"application/json",buffer:Buffer.from(JSON.stringify(data))});
  await page.waitForFunction(()=>document.querySelector("#setup-status").textContent.includes("loaded"));
 };
 const other={...setup.record,participantName:"Other Example"};
 await load(other);
 assert.equal(await page.locator("#participant-name").inputValue(),"Example Person");
 assert.match(await page.locator("#setup-status").textContent(),/Current session name kept/);
 await page.locator("#open-session").click();
 await page.locator("#new-session").click();
 await page.locator("#close-session").click();
 await load(other);
 assert.equal(await page.locator("#participant-name").inputValue(),"Other Example");
 // Returning and actually closing both restore the original canvas and preserve capture.
 await child.locator("#camera-window-return").click();
 await page.locator("#mirror-canvas").waitFor({state:"visible"});
 assert.equal(await page.evaluate(()=>document.querySelector("#mirror-canvas")===originalCanvas),true);
 assert.equal(await page.evaluate(()=>captureCalls),1);
 child=await open();await child.close();
 await page.locator("#mirror-canvas").waitFor({state:"visible"});
 assert.equal(await page.evaluate(()=>originalTrack.readyState),"live");
 // Show the control boxes before opening their camera-window action from fullscreen.
 await page.locator("#fullscreen-view").click();await page.waitForFunction(()=>!!document.fullscreenElement);
 await page.locator("#toggle-controls").click();
 child=await open();
 await page.waitForFunction(()=>!document.fullscreenElement);
 assert.equal(await page.locator("#controls").isVisible(),true);
 await page.setViewportSize({width:390,height:650});
 await page.locator("#open-session").click();
 await page.locator("#participant-name").fill("");
 await page.locator("[data-score='0']").click();await page.locator("#start-session").click();
 if(process.env.ARM_SCREENSHOT_DIR) await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/session-window.png`});
 await child.locator("#camera-window-stop").click();
 assert.equal(await page.evaluate(()=>originalTrack.readyState),"ended");
 assert.match(await page.locator("#session-summary").textContent(),/Session active/);
 await child.locator("#camera-window-end").click();
 const anonymous=await download(()=>child.keyboard.press("2"));
 assert.equal(anonymous.record.participantName,"");assert.match(anonymous.name,/^\d{4}-/);

 const emptySetup=await download(()=>page.locator("#save-setup").click());
 assert.equal(emptySetup.record.participantName,"");assert.match(emptySetup.name,/^\d{4}-.*_setup\.json$/);
 const closing=child.waitForEvent("close");await page.close();await closing;
 console.log("PASS Name restoration and session-name protection, anonymous filenames, return/close/reopen, fullscreen detachment, camera stop and parent-close cleanup");
 const blocked=await context.newPage();await blocked.addInitScript(()=>{window.open=()=>null;});await blocked.goto(base);
 await blocked.locator("#open-camera-window").click();
 assert.match(await blocked.locator("#camera-window-notice").textContent(),/blocked/);
 assert.equal(await blocked.locator(".preview-panel").isVisible(),true);
 const standalone=await context.newPage();await standalone.goto(new URL("camera.html",base.endsWith("/")?base:base+"/").href);
 assert.equal(await standalone.locator("#camera-window-help").isVisible(),true);
 assert.equal(await standalone.evaluate(()=>captureCalls),0);
 assert.deepEqual(errors,[]);
 console.log(`PASS Pop-up-blocked fallback, standalone camera-page recovery and no script errors. Chrome ${browser.version()}`);
} finally {await browser.close();}
