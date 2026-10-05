import test from "node:test";
import assert from "node:assert/strict";
import { MirrorView } from "../src/mirrorView.js";

test("mirror reverses x once, preserves dimensions, and Stop clears the frame and loop", t => {
  const transforms = [], cancelled = [], drawings = [];
  let nextFrame;
  const originalRequest = globalThis.requestAnimationFrame;
  const originalCancel = globalThis.cancelAnimationFrame;
  globalThis.requestAnimationFrame = callback => { nextFrame = callback; return 17; };
  globalThis.cancelAnimationFrame = id => cancelled.push(id);
  t.after(() => {
    if (originalRequest) globalThis.requestAnimationFrame = originalRequest; else delete globalThis.requestAnimationFrame;
    if (originalCancel) globalThis.cancelAnimationFrame = originalCancel; else delete globalThis.cancelAnimationFrame;
  });
  const context = { setTransform: (...args) => transforms.push(args), fillRect: (...args) => drawings.push(["clear", ...args]), drawImage: (...args) => drawings.push(args) };
  const canvas = { width: 0, height: 0, getContext: () => context };
  const video = { videoWidth: 1280, videoHeight: 720, readyState: 4 };
  const view = new MirrorView(canvas, video);
  view.start();
  assert.equal(canvas.width, 1280);
  assert.equal(canvas.height, 720);
  assert.deepEqual(transforms.at(-1), [-1, 0, 0, 1, 1280, 0]);
  assert.deepEqual(drawings.at(-1), [video, 0, 0, 1280, 720]);
  view.mirrored = false;
  nextFrame();
  assert.deepEqual(transforms.at(-1), [1, 0, 0, 1, 0, 0]);
  view.stop();
  assert.deepEqual(cancelled, [17]);
  assert.deepEqual(drawings.at(-1), ["clear", 0, 0, 1280, 720]);
});
