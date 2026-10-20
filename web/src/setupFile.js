// A small, versioned setup document. It is not a study session or pain record.
import { REGION_IDS, defaultCenter, validateCenter, defaultRegions, validateRegions } from "./mirrorRegions.js";
export const MAX_SETUP_BYTES = 64 * 1024;
const format = "arm-rgb-setup";

export function createSetup({ name = "", participantName = "", mirrored = true, cameraLabel = "", regions = defaultRegions(), center = defaultCenter() }, date = new Date()) {
  return validateSetup({
    format, schemaVersion: 4, savedAt: date.toISOString(),
    name: name.trim(), participantName: participantName.trim(), camera: { preferredLabel: cameraLabel }, view: { mirrored, regions, center },
  });
}

export function parseSetup(text) {
  if (new TextEncoder().encode(text).length > MAX_SETUP_BYTES) {
    throw new Error("This file is too large for a camera setup. Choose an ARM browser setup JSON file.");
  }
  let json;
  try { json = JSON.parse(text); }
  catch { throw new Error("This file is not valid JSON. Choose a setup saved by this browser app."); }
  const setup = validateSetup(json);
  if (json.schemaVersion < 3) Object.defineProperty(setup, "importNotice", { value: json.schemaVersion === 2
    ? "Older setup loaded. Camera direction and left/right choices kept. Top/bottom flips and individual offsets removed; shared alignment centered."
    : "Older setup loaded. Camera direction kept; quadrant choices and shared alignment start centered." });
  return setup;
}

function validateSetup(json) {
  if (!json || json.format !== format) {
    throw new Error("Choose an ARM browser setup file. Desktop study sessions are not supported in this first browser version.");
  }
  if (![1, 2, 3, 4].includes(json.schemaVersion)) {
    throw new Error("This setup uses a different version. Open it with the version of ARM that saved it.");
  }
  if (typeof json.name !== "string" || json.name.length > 80
    || (json.schemaVersion === 4 && (typeof json.participantName !== "string" || json.participantName.length > 80))
    || typeof json.camera?.preferredLabel !== "string" || json.camera.preferredLabel.length > 256
    || typeof json.view?.mirrored !== "boolean"
    || typeof json.savedAt !== "string" || !Number.isFinite(Date.parse(json.savedAt))) {
    throw new Error("The setup is missing valid settings. Your current setup has not been changed.");
  }
  if (json.schemaVersion === 2) {
    // Validate the old format before explicitly discarding its retired settings.
    validateRegions(json.view.regions);
    for (const id of REGION_IDS) {
      const region = json.view.regions[id];
      if (typeof region.mirrorY !== "boolean" || !Number.isFinite(region.offsetX) || Math.abs(region.offsetX) > 50
        || !Number.isFinite(region.offsetY) || Math.abs(region.offsetY) > 50) {
        throw new Error("The setup has invalid quadrant settings. Your current setup has not been changed.");
      }
    }
  }
  // Copy known fields, rather than mixing an imported object into live application state.
  return {
    format, schemaVersion: 4, savedAt: json.savedAt, name: json.name, participantName: json.schemaVersion === 4 ? json.participantName : "",
    camera: { preferredLabel: json.camera.preferredLabel },
    view: { mirrored: json.view.mirrored, regions: json.schemaVersion === 1 ? defaultRegions() : validateRegions(json.view.regions),
      center: json.schemaVersion >= 3 ? validateCenter(json.view.center) : defaultCenter() },
  };
}

export function setupFilename(setup) {
  const name = setup.participantName.normalize("NFC")
    .replace(/[^\p{L}\p{N}_-]+/gu, "_").replace(/^_+|_+$/g, "");
  // Limit bytes too: filenames on common filesystems have a 255-byte component limit.
  let prefix = "";
  for (const character of name) {
    if (new TextEncoder().encode(prefix + character).length > 120) break;
    prefix += character;
  }
  const timestamp = new Date(setup.savedAt).toISOString().replace(/[:.]/g, "-");
  return `${prefix ? prefix + "_" : ""}${timestamp}_setup.json`;
}

export function downloadSetup(setup) {
  const url = URL.createObjectURL(new Blob([JSON.stringify(setup, null, 2) + "\n"], { type: "application/json" }));
  const link = document.createElement("a");
  link.href = url;
  link.download = setupFilename(setup);
  document.body.append(link);
  link.click();
  link.remove();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
}
