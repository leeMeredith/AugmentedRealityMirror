import test from "node:test";
import assert from "node:assert/strict";
import { createSetup, parseSetup, setupFilename, MAX_SETUP_BYTES } from "../src/setupFile.js";

const date = new Date("2026-10-05T12:34:56.789Z");
test("named settings round trip without requiring participant or administrator fields", () => {
  const setup = createSetup({ name: "  home chair  ", mirrored: false, cameraLabel: "USB Camera" }, date);
  assert.deepEqual(parseSetup(JSON.stringify(setup)), setup);
  assert.equal(setup.name, "home chair");
  assert.equal(setupFilename(setup), "home_chair_setup_2026-10-05T12-34-56-789Z.json");
  assert.equal(createSetup({}).view.mirrored, true);
});
test("invalid, unrelated and future files are rejected rather than partially restored", () => {
  assert.throws(() => parseSetup("{"), /valid JSON/);
  assert.throws(() => parseSetup('{"session":{"participantId":"Example"}}'), /Desktop study sessions/);
  assert.throws(() => parseSetup("null"), /browser setup/);
  const setup = createSetup({}, date);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, schemaVersion: 2 })), /different version/);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, view: { mirrored: "false" } })), /valid settings/);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, savedAt: "nonsense" })), /valid settings/);
});
test("unknown fields are not mixed into the live settings", () => {
  const setup = createSetup({}, date);
  assert.deepEqual(parseSetup(JSON.stringify({ ...setup, administratorId: "ignored", view: { mirrored: true, arbitrary: 1 } })), setup);
});
test("existing setups retain both saved flip choices independently of the startup default", () => {
  for (const mirrored of [true, false]) {
    const oldSetup = { format: "arm-rgb-setup", schemaVersion: 1, savedAt: date.toISOString(), name: "old setup", camera: { preferredLabel: "" }, view: { mirrored } };
    assert.equal(parseSetup(JSON.stringify(oldSetup)).view.mirrored, mirrored);
  }
});
test("oversized imports are rejected", () => {
  assert.throws(() => parseSetup(" ".repeat(MAX_SETUP_BYTES + 1)), /too large/);
});
test("filenames keep readable Unicode names while removing path characters", () => {
  const setup = createSetup({ name: "../Zoë / chair:*?" }, date);
  assert.equal(setupFilename(setup), "Zoë_chair_setup_2026-10-05T12-34-56-789Z.json");
  assert.match(setupFilename(createSetup({ name: "...///" }, date)), /^ARM_setup_/);
  assert.ok(new TextEncoder().encode(setupFilename(createSetup({ name: "椅".repeat(80) }, date))).length < 255);
});
