// Optional integration checks. The application itself has no Playwright dependency.
import assert from "node:assert/strict";
const { chromium } = await import(process.env.ARM_PLAYWRIGHT_MODULE || "playwright");
const browser = await chromium.launch({
  headless: true,
  ...(process.env.ARM_CHROME_EXECUTABLE ? { executablePath: process.env.ARM_CHROME_EXECUTABLE } : {}),
  args: ["--use-fake-device-for-media-stream", "--use-fake-ui-for-media-stream"],
});
const url = process.env.ARM_TEST_URL || "http://localhost:8001";
const results = [];
const passed = name => { results.push(name); console.log(`PASS ${name}`); };

try {
  const context = await browser.newContext({ viewport: { width: 1360, height: 1000 } });
  const page = await context.newPage();
  const errors = [];
  page.on("pageerror", error => errors.push(error.message));
  await page.goto(url);
  assert.equal(await page.locator("#build-version").textContent(), "Browser build 7");
  await page.locator("#start-camera").click();
  await page.waitForFunction(() => document.querySelector("#camera-state").textContent === "Camera on");
  await page.evaluate(() => { window.originalTrack = document.querySelector("#camera-video").srcObject.getVideoTracks()[0]; });
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => document.fullscreenElement?.id === "mirror-workspace");
  assert.equal(await page.locator("#controls").isHidden(), true);
  assert.equal(await page.locator("#stop-view").isVisible(), true);
  await page.waitForFunction(() => document.querySelector("#mirror-canvas").getContext("2d").getTransform().a === -1);
  assert.equal(await page.evaluate(() => window.originalTrack.readyState), "live");
  passed("Native fullscreen hides settings and preserves live camera and orientation");

  const bounds = await page.evaluate(() => {
    const frame = document.querySelector("#mirror-canvas");
    const rect = frame.getBoundingClientRect();
    return { left: rect.left, right: rect.right, top: rect.top, bottom: rect.bottom, width: innerWidth, height: innerHeight, fit: getComputedStyle(frame).objectFit };
  });
  assert.ok(bounds.left >= 0 && bounds.right <= bounds.width && bounds.top >= 0 && bounds.bottom <= bounds.height);
  assert.equal(bounds.fit, "contain");
  passed("Entire canvas fits the fullscreen viewport with aspect ratio preserved");
  if (process.env.ARM_SCREENSHOT_DIR) await page.screenshot({ path: `${process.env.ARM_SCREENSHOT_DIR}/fullscreen.png` });

  await page.locator("#toggle-controls").click();
  assert.equal(await page.locator("#controls").isVisible(), true);
  assert.equal(await page.locator("#toggle-controls").getAttribute("aria-expanded"), "true");
  await page.locator("#reverse-view").click();
  await page.waitForFunction(() => document.querySelector("#mirror-canvas").getContext("2d").getTransform().a === 1);
  await page.locator("#toggle-controls").click();
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !document.fullscreenElement && !document.querySelector("#mirror-workspace").classList.contains("is-expanded"));
  assert.equal(await page.locator("#controls").isVisible(), true);
  assert.equal(await page.evaluate(() => window.originalTrack.readyState), "live");
  assert.equal(await page.evaluate(() => document.activeElement.id), "fullscreen-view");
  passed("Controls work in fullscreen; button exit restores layout, focus and earlier panel visibility");

  await page.locator("#toggle-controls").click();
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !!document.fullscreenElement);
  await page.keyboard.press("Escape");
  await page.waitForFunction(() => !document.fullscreenElement && !document.querySelector("#mirror-workspace").classList.contains("is-expanded"));
  assert.equal(await page.locator("#controls").isHidden(), true);
  passed("Escape exits and preserves a panel that was hidden before entry");

  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !!document.fullscreenElement);
  await page.locator("#stop-view").click();
  assert.equal(await page.evaluate(() => window.originalTrack.readyState), "ended");
  assert.equal(await page.locator("#mirror-canvas").isHidden(), true);
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !document.fullscreenElement && !document.querySelector("#mirror-workspace").classList.contains("is-expanded"));
  passed("Top-toolbar Stop releases capture with settings hidden");

  // Browser-driven exits must also update the app, without relying on its exit button.
  await page.locator("#fullscreen-view").click();
  await page.waitForFunction(() => !!document.fullscreenElement);
  await page.evaluate(() => document.exitFullscreen());
  await page.waitForFunction(() => !document.querySelector("#mirror-workspace").classList.contains("is-expanded"));
  passed("Browser-driven fullscreen exit restores the page");

  for (const mode of ["unavailable", "rejected"]) {
    const fallback = await context.newPage();
    fallback.on("pageerror", error => errors.push(error.message));
    await fallback.addInitScript(mode => {
      if (mode === "unavailable") Object.defineProperty(document, "fullscreenEnabled", { value: false });
      else Element.prototype.requestFullscreen = () => Promise.reject(new Error("Blocked for test"));
    }, mode);
    await fallback.goto(url);
    await fallback.locator("#fullscreen-view").click();
    await fallback.waitForFunction(() => document.querySelector("#fullscreen-view").textContent === "Exit expanded view" && !document.querySelector("#fullscreen-view").disabled);
    assert.equal(await fallback.evaluate(() => document.fullscreenElement), null);
    assert.equal(await fallback.locator("#controls").isHidden(), true);
    assert.equal(await fallback.locator(".masthead").evaluate(element => element.inert), true);
    await fallback.keyboard.press("Escape");
    assert.equal(await fallback.locator(".masthead").evaluate(element => element.inert), false);
    assert.equal(await fallback.locator("#controls").isVisible(), true);
    await fallback.locator("#fullscreen-view").click();
    await fallback.locator("#fullscreen-view").click();
    assert.equal(await fallback.locator("#mirror-workspace").evaluate(element => element.classList.contains("is-expanded")), false);
    passed(`Expanded fallback enters and exits when fullscreen is ${mode}`);

    await fallback.setViewportSize({ width: 390, height: 600 });
    await fallback.locator("#fullscreen-view").click();
    await fallback.locator("#toggle-controls").click();
    const fit = await fallback.locator("#controls").evaluate(element => {
      const rect = element.getBoundingClientRect();
      const toolbar = document.querySelector(".view-toolbar").getBoundingClientRect();
      return rect.left >= 0 && rect.right <= innerWidth && rect.top >= toolbar.bottom && rect.bottom <= innerHeight && element.scrollHeight > element.clientHeight;
    });
    assert.equal(fit, true);
    assert.equal(await fallback.locator("#fullscreen-view").isVisible(), true);
    if (mode === "rejected" && process.env.ARM_SCREENSHOT_DIR) await fallback.screenshot({ path: `${process.env.ARM_SCREENSHOT_DIR}/narrow.png` });
    await fallback.locator("#fullscreen-view").click();
    await fallback.close();
  }
  passed("Narrow layout keeps panel scrollable and exits accessible");
  assert.deepEqual(errors, []);
  console.log(`${results.length} browser checks passed (${browser.version()}); synthetic camera only.`);
} finally { await browser.close(); }
