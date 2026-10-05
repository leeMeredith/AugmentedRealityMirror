import test from "node:test";
import assert from "node:assert/strict";
import { CaptureRGB } from "../src/captureRGB.js";

function environment(t, getUserMedia) {
  const savedNavigator = Object.getOwnPropertyDescriptor(globalThis, "navigator");
  const savedSecure = Object.getOwnPropertyDescriptor(globalThis, "isSecureContext");
  Object.defineProperty(globalThis, "navigator", { configurable: true, value: { mediaDevices: { getUserMedia } } });
  Object.defineProperty(globalThis, "isSecureContext", { configurable: true, value: true });
  t.after(() => {
    if (savedNavigator) Object.defineProperty(globalThis, "navigator", savedNavigator); else delete globalThis.navigator;
    if (savedSecure) Object.defineProperty(globalThis, "isSecureContext", savedSecure); else delete globalThis.isSecureContext;
  });
  const video = { srcObject: null, play: async () => {}, pause() {} };
  return { video, capture: new CaptureRGB(video) };
}
function stream() {
  const track = new EventTarget();
  track.label = "Test camera";
  track.readyState = "live";
  track.stopped = false;
  track.stop = () => { track.stopped = true; track.readyState = "ended"; };
  track.getSettings = () => ({ deviceId: "test-camera" });
  return { track, getTracks: () => [track], getVideoTracks: () => [track] };
}
function deferred() {
  let resolve, reject;
  const promise = new Promise((yes, no) => { resolve = yes; reject = no; });
  return { promise, resolve, reject };
}

test("a late permission response is released after Stop", async t => {
  const pending = deferred();
  const { capture, video } = environment(t, () => pending.promise);
  const result = capture.start();
  capture.stop();
  const late = stream();
  pending.resolve(late);
  assert.equal(await result, false);
  assert.equal(late.track.stopped, true);
  assert.equal(capture.active, false);
  assert.equal(video.srcObject, null);
});
test("a stale request cannot replace a newer camera", async t => {
  const first = deferred();
  const second = stream();
  let count = 0;
  const { capture, video } = environment(t, () => ++count === 1 ? first.promise : Promise.resolve(second));
  const stale = capture.start();
  assert.equal(await capture.start("test-camera"), true);
  const oldStream = stream();
  first.resolve(oldStream);
  assert.equal(await stale, false);
  assert.equal(oldStream.track.stopped, true);
  assert.equal(video.srcObject, second);
  assert.equal(second.track.stopped, false);
  capture.stop();
});
test("permission denial is actionable and leaves capture off", async t => {
  const { capture } = environment(t, async () => { throw { name: "NotAllowedError" }; });
  await assert.rejects(capture.start(), /privacy settings/);
  assert.equal(capture.active, false);
});
test("stream is released when video playback fails", async t => {
  const media = stream();
  const { capture, video } = environment(t, async () => media);
  video.play = async () => { throw new Error("Playback failed"); };
  await assert.rejects(capture.start(), /Playback failed/);
  assert.equal(media.track.stopped, true);
  assert.equal(video.srcObject, null);
});
test("switching releases the old stream and requests video only", async t => {
  const first = stream(), second = stream();
  let count = 0, constraints;
  const { capture } = environment(t, async options => { constraints = options; return ++count === 1 ? first : second; });
  await capture.start();
  await capture.start("second");
  assert.equal(first.track.stopped, true);
  assert.equal(constraints.audio, false);
  assert.deepEqual(constraints.video.deviceId, { exact: "second" });
  capture.stop();
  assert.equal(second.track.stopped, true);
});
test("disconnection clears capture and calls the application once", async t => {
  const media = stream();
  const { capture, video } = environment(t, async () => media);
  let ended = 0;
  capture.onEnded = () => { ended++; };
  await capture.start();
  media.track.dispatchEvent(new Event("ended"));
  assert.equal(ended, 1);
  assert.equal(capture.active, false);
  assert.equal(video.srcObject, null);
});
