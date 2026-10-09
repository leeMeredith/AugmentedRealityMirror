// Optional headless checks; uses synthetic video, never a physical camera.
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
  assert.equal(await page.locator("#build-version").textContent(), "Browser build 6");
  const pixels = await page.evaluate(async () => {
    const { drawRegions, defaultRegions, REGION_IDS } = await import("./src/mirrorRegions.js");
    const source = document.createElement("canvas"), target = document.createElement("canvas");
    source.width = target.width = 80; source.height = target.height = 60;
    const input = source.getContext("2d"), output = target.getContext("2d");
    output.imageSmoothingEnabled = false;
    const image = input.createImageData(80, 60);
    for (let y = 0; y < 60; y++) for (let x = 0; x < 80; x++) {
      image.data.set([x * 3, y * 4, (x + y) % 256, 255], (y * 80 + x) * 4);
    }
    input.putImageData(image, 0, 0);
    let cases = 0;
    for (const mirrored of [false, true]) for (let selected = 0; selected < 4; selected++) {
      for (const mirrorX of [false, true]) for (const mirrorY of [false, true]) {
        for (const [offsetX, offsetY] of [[0, 0], [25, -50], [-50, 50]]) {
          const regions = defaultRegions();
          regions[REGION_IDS[selected]] = { mirrorX, mirrorY, offsetX, offsetY };
          drawRegions(output, source, 80, 60, mirrored, regions);
          const actual = output.getImageData(0, 0, 80, 60).data;
          for (let y = 0; y < 60; y++) for (let x = 0; x < 80; x++) {
            const region = Math.floor(y / 30) * 2 + Math.floor(x / 40);
            let sx = x, sy = y, blank = false;
            if (region === selected) {
              sx -= offsetX * 40 / 100; sy -= offsetY * 30 / 100;
              blank = sx < (selected % 2) * 40 || sx >= (selected % 2 + 1) * 40
                || sy < Math.floor(selected / 2) * 30 || sy >= (Math.floor(selected / 2) + 1) * 30;
              if (mirrorX) sx = 79 - sx;
              if (mirrorY) sy = 59 - sy;
            }
            if (mirrored) sx = 79 - sx;
            const expected = blank ? [20, 47, 44, 255] : [sx * 3, sy * 4, (sx + sy) % 256, 255];
            const index = (y * 80 + x) * 4;
            if (expected.some((value, channel) => value !== actual[index + channel])) {
              return { error: { mirrored, selected, mirrorX, mirrorY, offsetX, offsetY, x, y, expected, actual: [...actual.slice(index, index + 4)] } };
            }
          }
          cases++;
        }
      }
    }
    return { cases };
  });
  assert.deepEqual(pixels, { cases: 96 });
  console.log("PASS 96 pixel cases: both camera orientations, every quadrant/toggle combination, offsets, clipping and unchanged neighbors");

  await page.locator("#start-camera").click();
  await page.waitForFunction(() => document.querySelector("#camera-state").textContent === "Camera on");
  await page.evaluate(() => { window.originalTrack = document.querySelector("#camera-video").srcObject.getVideoTracks()[0]; });
  await page.locator("#region-mirror-x").check();
  await page.locator("#region-offset-x").fill("25");
  await page.locator("[data-region=bottomRight]").click();
  assert.equal(await page.locator("#region-mirror-x").isChecked(), false);
  assert.equal(await page.locator("#region-offset-x").inputValue(), "0");
  await page.locator("#region-mirror-y").check();
  await page.locator("#region-offset-y").fill("-30");
  await page.locator("[data-region=topLeft]").click();
  assert.equal(await page.locator("#region-mirror-x").isChecked(), true);
  assert.equal(await page.locator("#region-offset-x").inputValue(), "25");
  await page.locator("#setup-name").fill("Example chair");
  const downloadPromise = page.waitForEvent("download");
  await page.locator("#save-setup").click();
  const download = await downloadPromise;
  const saved = JSON.parse(await readFile(await download.path(), "utf8"));
  assert.match(download.suggestedFilename(), /^Example_chair_setup_/);
  assert.equal(saved.schemaVersion, 2);
  assert.deepEqual(saved.view.regions.topLeft, { mirrorX: true, mirrorY: false, offsetX: 25, offsetY: 0 });
  assert.deepEqual(saved.view.regions.bottomRight, { mirrorX: false, mirrorY: true, offsetX: 0, offsetY: -30 });
  await page.locator("#reset-region").click();
  assert.equal(await page.locator("#region-mirror-x").isChecked(), false);
  assert.equal(await page.locator("#region-offset-x").inputValue(), "0");
  await page.locator("[data-region=bottomRight]").click();
  assert.equal(await page.locator("#region-offset-y").inputValue(), "-30");
  const load = async data => {
    await page.locator("#setup-status").evaluate(element => { element.textContent = ""; });
    await page.locator("#setup-file").setInputFiles({ name: "setup.json", mimeType: "application/json", buffer: Buffer.from(JSON.stringify(data)) });
    await page.waitForFunction(() => document.querySelector("#setup-status").textContent !== "");
  };
  await load(saved);
  await page.locator("[data-region=topLeft]").click();
  assert.equal(await page.locator("#region-offset-x").inputValue(), "25");
  const bad = structuredClone(saved);
  bad.view.regions.topLeft.offsetX = 500;
  await load(bad);
  assert.match(await page.locator("#setup-status").textContent(), /invalid quadrant/);
  assert.equal(await page.locator("#region-offset-x").inputValue(), "25");
  console.log("PASS Independent quadrant controls, reset, named JSON round trip and invalid import preservation");

  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !!document.fullscreenElement);
  await page.locator("#toggle-controls").click();
  await page.locator("#region-offset-x").fill("-10");
  assert.equal(await page.evaluate(() => window.originalTrack === document.querySelector("#camera-video").srcObject.getVideoTracks()[0] && window.originalTrack.readyState === "live"), true);
  if (process.env.ARM_SCREENSHOT_DIR) await page.screenshot({ path: `${process.env.ARM_SCREENSHOT_DIR}/regions-fullscreen.png` });
  await page.keyboard.press("Escape");
  await page.waitForFunction(() => !document.fullscreenElement);
  await load({ ...saved, schemaVersion: 1, view: { mirrored: false } });
  for (const id of ["topLeft", "topRight", "bottomLeft", "bottomRight"]) {
    await page.locator(`[data-region=${id}]`).click();
    assert.equal(await page.locator("#region-mirror-x").isChecked(), false);
    assert.equal(await page.locator("#region-mirror-y").isChecked(), false);
    assert.equal(await page.locator("#region-offset-x").inputValue(), "0");
    assert.equal(await page.locator("#region-offset-y").inputValue(), "0");
  }
  await page.waitForFunction(() => document.querySelector("#mirror-canvas").getContext("2d").getTransform().a === 1);
  console.log("PASS Fullscreen adjustments preserve capture; version 1 imports clear regions and retain original orientation");
  await page.setViewportSize({ width: 390, height: 600 });
  await page.locator("#region-offset-y").focus();
  await page.keyboard.press("ArrowRight");
  assert.equal(await page.locator("#region-offset-y").inputValue(), "1");
  assert.equal(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), true);
  if (process.env.ARM_SCREENSHOT_DIR) await page.screenshot({ path: `${process.env.ARM_SCREENSHOT_DIR}/regions-narrow.png` });
  await page.locator("#stop-view").click();
  assert.equal(await page.evaluate(() => window.originalTrack.readyState), "ended");
  assert.deepEqual(errors, []);
  console.log(`PASS Narrow layout, keyboard slider and capture stop; Chrome ${browser.version()}`);
} finally { await browser.close(); }
