import test from "node:test";
import assert from "node:assert/strict";
import { createSetup, parseSetup, setupFilename, MAX_SETUP_BYTES } from "../src/setupFile.js";
import { defaultRegions } from "../src/mirrorRegions.js";

const date = new Date("2026-10-05T12:34:56.789Z");
test("named settings round trip without requiring participant or administrator fields", () => {
  const setup = createSetup({ name: "  home chair  ", participantName: "Example Person", mirrored: false, cameraLabel: "USB Camera" }, date);
  assert.deepEqual(parseSetup(JSON.stringify(setup)), setup);
  assert.equal(setup.name, "home chair");
  assert.equal(setupFilename(setup), "Example_Person_2026-10-05T12-34-56-789Z_setup.json");
  assert.equal(createSetup({}).view.mirrored, true);
});
test("invalid, unrelated and future files are rejected rather than partially restored", () => {
  assert.throws(() => parseSetup("{"), /valid JSON/);
  assert.throws(() => parseSetup('{"session":{"participantId":"Example"}}'), /Desktop study sessions/);
  assert.throws(() => parseSetup("null"), /browser setup/);
  const setup = createSetup({}, date);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, schemaVersion: 5 })), /different version/);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, view: { mirrored: "false" } })), /valid settings/);
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, savedAt: "nonsense" })), /valid settings/);
});
test("unknown fields are not mixed into the live settings", () => {
  const setup = createSetup({}, date);
  assert.deepEqual(parseSetup(JSON.stringify({ ...setup, administratorId: "ignored", view: { ...setup.view, arbitrary: 1 } })), setup);
});
test("existing setups retain both saved flip choices independently of the startup default", () => {
  for (const mirrored of [true, false]) {
    const oldSetup = { format: "arm-rgb-setup", schemaVersion: 1, savedAt: date.toISOString(), name: "old setup", camera: { preferredLabel: "" }, view: { mirrored } };
    assert.equal(parseSetup(JSON.stringify(oldSetup)).view.mirrored, mirrored);
    assert.deepEqual(parseSetup(JSON.stringify(oldSetup)).view.regions, defaultRegions());
  }
});
test("schema 4 preserves shared alignment and independent mirror choices", () => {
  const regions = defaultRegions();
  regions.topLeft.mirrorX = true;
  regions.bottomRight.mirrorX = true;
  const setup = createSetup({ regions, center: { x: 32, y: 68 } }, date);
  assert.equal(setup.schemaVersion, 4);
  assert.deepEqual(parseSetup(JSON.stringify(setup)), setup);
  regions.topLeft.mirrorX = false;
  assert.equal(setup.view.regions.topLeft.mirrorX, true);
  for (const center of [undefined, {x: 0, y: 50}, {x: 50, y: 91}, {x: "50", y: 50}]) {
    assert.throws(() => parseSetup(JSON.stringify({ ...setup, view: { ...setup.view, center } })), /invalid alignment/);
  }
  assert.throws(() => parseSetup(JSON.stringify({ ...setup, view: {...setup.view, regions: {} } })), /invalid quadrant/);
});
test("schema 2 migration explicitly removes upside-down flips and individual offsets", () => {
  const old = createSetup({ mirrored: false }, date);
  old.schemaVersion = 2;
  old.view.regions = Object.fromEntries(Object.keys(old.view.regions).map(id => [id, { mirrorX: id === "topLeft", mirrorY: true, offsetX: 17, offsetY: -23 }]));
  delete old.view.center;
  const migrated = parseSetup(JSON.stringify(old));
  assert.equal(migrated.view.mirrored, false);
  assert.deepEqual(migrated.view.center, { x: 50, y: 50 });
  assert.deepEqual(migrated.view.regions.topLeft, { mirrorX: true });
  assert.match(migrated.importNotice, /Top\/bottom flips and individual offsets removed/);
  assert.equal(JSON.stringify(migrated).includes("importNotice"), false);
  old.view.regions.topLeft.offsetX = 51;
  assert.throws(() => parseSetup(JSON.stringify(old)), /invalid quadrant/);
});
test("oversized imports are rejected", () => {
  assert.throws(() => parseSetup(" ".repeat(MAX_SETUP_BYTES + 1)), /too large/);
});
test("filenames keep readable Unicode names while removing path characters", () => {
  const setup = createSetup({ participantName: "../Zoë / chair:*?" }, date);
  assert.equal(setupFilename(setup), "Zoë_chair_2026-10-05T12-34-56-789Z_setup.json");
  assert.match(setupFilename(createSetup({ participantName: "...///" }, date)), /^2026-/);
  assert.ok(new TextEncoder().encode(setupFilename(createSetup({ participantName: "椅".repeat(80) }, date))).length < 255);
});

test("schema 3 labels do not become participant names; schema 4 validates participant names", () => {
  const legacy = createSetup({name:"home chair",participantName:"not a legacy field",center:{x:32,y:68}},date);
  legacy.schemaVersion=3;
  const migrated=parseSetup(JSON.stringify(legacy));
  assert.equal(migrated.participantName,"");
  assert.equal(migrated.name,"home chair");
  assert.deepEqual(migrated.view.center,{x:32,y:68});
  const current=createSetup({participantName:"Example Person"},date);
  assert.equal(parseSetup(JSON.stringify(current)).participantName,"Example Person");
  assert.throws(()=>parseSetup(JSON.stringify({...current,participantName:32})),/valid settings/);
  assert.throws(()=>parseSetup(JSON.stringify({...current,participantName:"x".repeat(81)})),/valid settings/);
});
