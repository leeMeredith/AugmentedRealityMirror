import test from "node:test";
import assert from "node:assert/strict";
import { MirrorView } from "../src/mirrorView.js";

test("startup reflects the incoming frame, reversing twice restores it, and Stop clears the frame and loop", t => {
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
  assert.equal(view.mirrored, true);
  assert.deepEqual(transforms.at(-1), [-1, 0, 0, 1, 1280, 0]);
  assert.deepEqual(drawings.at(-1), [video, 0, 0, 1280, 720]);
  view.mirrored = !view.mirrored;
  nextFrame();
  assert.deepEqual(transforms.at(-1), [1, 0, 0, 1, 0, 0]);
  view.mirrored = !view.mirrored;
  nextFrame();
  assert.deepEqual(transforms.at(-1), [-1, 0, 0, 1, 1280, 0]);
  view.stop();
  assert.deepEqual(cancelled, [17]);
  assert.deepEqual(drawings.at(-1), ["clear", 0, 0, 1280, 720]);
});

test("moving the canvas transfers animation scheduling without restarting capture or leaving a second loop", () => {
  const cancelled = [], scheduled = [], drawings = [];
  const scheduler = name => ({requestAnimationFrame: () => {scheduled.push(name);return name;},cancelAnimationFrame: id => cancelled.push(id)});
  const first = scheduler("main"), second = scheduler("camera");
  const context = {setTransform() {},fillRect() {},drawImage: video => drawings.push(video)};
  const canvas = {width:80,height:60,ownerDocument:{defaultView:first},getContext:()=>context};
  const video = {videoWidth:80,videoHeight:60,readyState:4};
  const view = new MirrorView(canvas,video);
  view.start();
  view.moveCanvas({append: moved => {assert.equal(moved,canvas);canvas.ownerDocument={defaultView:second};}});
  assert.deepEqual(scheduled,["main","camera"]);
  assert.deepEqual(cancelled,["main"]);
  assert.deepEqual(drawings,[video,video]);
  view.stop();
  assert.deepEqual(cancelled,["main","camera"]);
});
