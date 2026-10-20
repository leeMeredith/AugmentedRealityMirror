// Optional browser checks; all participant data and video are synthetic.
import assert from "node:assert/strict";
import {readFile} from "node:fs/promises";
const {chromium}=await import(process.env.ARM_PLAYWRIGHT_MODULE||"playwright");
const browser=await chromium.launch({headless:true,
 ...(process.env.ARM_CHROME_EXECUTABLE?{executablePath:process.env.ARM_CHROME_EXECUTABLE}:{}),
 args:["--use-fake-device-for-media-stream","--use-fake-ui-for-media-stream"],
});
try {
 const page=await browser.newPage({viewport:{width:1440,height:1000}});
 const errors=[];page.on("pageerror",error=>errors.push(error.message));
 await page.goto(process.env.ARM_TEST_URL||"http://localhost:8001");
 assert.equal(await page.locator("#build-version").textContent(),"Browser build 9");
 assert.equal(await page.locator("#start-session").isDisabled(),true);
 await page.locator("#participant-name").fill("Example 123");
 await page.keyboard.press("0");
 assert.equal(await page.locator("#start-session").isDisabled(),true);
 await page.locator("#participant-name").fill("Example Participant");
 await page.locator("#loss-right-arm").check();
 await page.locator("#loss-right-arm-level").fill("below elbow");
 await page.locator("#loss-left-leg").check();
 await page.locator("#loss-left-leg-level").fill("above knee");
 await page.locator("[data-score='6']").click();
 await page.locator("#start-camera").click();
 await page.waitForFunction(()=>document.querySelector("#camera-state").textContent==="Camera on");
 await page.locator("#center-x").fill("32");
 await page.locator("#start-session").click();
 assert.equal(await page.locator("#participant-name").isDisabled(),true);
 assert.equal(await page.locator("#pain-history li").count(),1);
 await page.keyboard.press("4");
 assert.equal(await page.locator("#pain-history li").count(),2);
 await page.keyboard.press("0");
 assert.match(await page.locator("#pain-history li").last().textContent(),/10\/10/);
 // Repeats/modifiers and number keys used in camera/alignment fields must not add scores.
 await page.evaluate(()=>document.dispatchEvent(new KeyboardEvent("keydown",{key:"7",repeat:true,bubbles:true})));
 await page.evaluate(()=>document.dispatchEvent(new KeyboardEvent("keydown",{key:"7",ctrlKey:true,bubbles:true})));
 await page.locator("#center-y").focus();await page.keyboard.press("8");
 assert.equal(await page.locator("#pain-history li").count(),3);
 await page.locator("#setup-name").fill("Example 123");await page.keyboard.press("0");
 assert.equal(await page.locator("#pain-history li").count(),3);
 const download=async action=>{
  const waiting=page.waitForEvent("download");await action();const item=await waiting;
  return {name:item.suggestedFilename(),record:JSON.parse(await readFile(await item.path(),"utf8"))};
 };
 const progress=await download(()=>page.locator("#save-session").click());
 assert.match(progress.name,/^Example_Participant_.*Z\.json$/);
 assert.equal(progress.record.status,"active");assert.equal(progress.record.endedAt,null);
 assert.deepEqual(progress.record.painEntries.map(e=>e.painScore),[6,4,10]);
 assert.deepEqual(progress.record.limbLoss,[{side:"right",limb:"arm",level:"below elbow"},{side:"left",limb:"leg",level:"above knee"}]);
 assert.equal(progress.record.viewSettings.center.x,32);
 console.log("PASS Named baseline/during scores, key 0 = 10, text-field/repeat/modifier protection and unfinished download");
 const warning=page.waitForEvent("dialog");
 const leaving=page.getByRole("link",{name:/Getting started/}).click();
 const dialog=await warning;assert.equal(dialog.type(),"beforeunload");await dialog.dismiss();await leaving;
 assert.match(await page.locator("#session-summary").textContent(),/Session active/);
 console.log("PASS Active-session navigation warning can cancel leaving without losing the session");
 await page.locator("#fullscreen-view").click();await page.waitForFunction(()=>!!document.fullscreenElement);
 assert.equal(await page.locator("#controls").isHidden(),true);
 await page.keyboard.press("2");
 await page.locator("#end-session-view").click();
 assert.match(await page.locator("#session-summary").textContent(),/final pain score/);
 const final=await download(()=>page.keyboard.press("3"));
 assert.equal(final.record.status,"completed");assert.ok(final.record.endedAt);
 assert.deepEqual(final.record.painEntries.map(e=>e.phase),["before","during","during","during","after"]);
 assert.deepEqual(final.record.painEntries.map(e=>e.painScore),[6,4,10,2,3]);
 assert.ok(final.record.painEntries.every(e=>Number.isFinite(e.elapsedSeconds)&&e.recordedAt.endsWith("Z")));
 assert.equal(final.record.endViewSettings.center.x,32);
 assert.match(await page.locator("#session-summary").textContent(),/Session ended/);
 await page.locator("#toggle-controls").click();
 const again=await download(()=>page.locator("#save-session").click());
 assert.deepEqual(again.record,final.record);
 console.log("PASS Fullscreen scoring/end controls, one-selection final score/download, repeat download and chart columns");
 await page.keyboard.press("Escape");await page.waitForFunction(()=>!document.fullscreenElement);
 await page.locator("#new-session").click();
 await page.locator("#participant-name").fill("");
 await page.locator("#loss-right-arm").uncheck();await page.locator("#loss-left-leg").uncheck();
 await page.locator("[data-score='0']").click();await page.locator("#start-session").click();
 // Camera Stop does not silently end or erase the session.
 await page.locator("#stop-view").click();
 assert.match(await page.locator("#session-summary").textContent(),/Session active/);
 await page.locator("#end-session").click();await page.locator("#continue-session").click();
 assert.equal(await page.locator("#pain-phase").textContent(),"Pain during use");
 await page.locator("#end-session").click();
 const anonymous=await download(()=>page.locator("[data-score='0']").click());
 assert.match(anonymous.name,/^\d{4}-\d{2}-\d{2}T.*Z\.json$/);
 assert.notEqual(anonymous.record.sessionId,final.record.sessionId);
 assert.deepEqual(anonymous.record.painEntries.map(e=>e.painScore),[0,0]);
 assert.deepEqual(anonymous.record.limbLoss,[]);
 await page.locator("#setup-file").setInputFiles({name:"session.json",mimeType:"application/json",buffer:Buffer.from(JSON.stringify(anonymous.record))});
 await page.waitForFunction(()=>document.querySelector("#setup-status").classList.contains("error"));
 assert.equal(await page.locator("#center-x").inputValue(),"32");
 console.log("PASS Anonymous date/time filenames, zero scores, new IDs, end cancellation, camera stop and setup/session separation");
 await page.setViewportSize({width:390,height:600});
 await page.locator("#pain-scores").scrollIntoViewIfNeeded();
 const fits=await page.evaluate(()=>{
  const v=document.querySelector("#mirror-canvas").parentElement.getBoundingClientRect(),c=document.querySelector("#controls").getBoundingClientRect();
  return v.height>0&&v.top>=0&&v.bottom<=innerHeight&&c.top>=v.bottom&&document.documentElement.scrollWidth<=innerWidth;
 });
 assert.equal(fits,true);
 if(process.env.ARM_SCREENSHOT_DIR) await page.screenshot({path:`${process.env.ARM_SCREENSHOT_DIR}/session-narrow.png`});
 assert.deepEqual(errors,[]);
 console.log(`PASS Narrow scrolling/session form and no script errors. Chrome ${browser.version()}`);
} finally {await browser.close();}
