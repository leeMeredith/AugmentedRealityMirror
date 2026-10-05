// A small, versioned setup document. It is not a study session or pain record.
export const MAX_SETUP_BYTES = 64 * 1024;
const format = "arm-rgb-setup";

export function createSetup({ name = "", mirrored = false, cameraLabel = "" }, date = new Date()) {
  return validateSetup({
    format, schemaVersion: 1, savedAt: date.toISOString(),
    name: name.trim(), camera: { preferredLabel: cameraLabel }, view: { mirrored },
  });
}

export function parseSetup(text) {
  if (new TextEncoder().encode(text).length > MAX_SETUP_BYTES) {
    throw new Error("This file is too large for a camera setup. Choose an ARM browser setup JSON file.");
  }
  let json;
  try { json = JSON.parse(text); }
  catch { throw new Error("This file is not valid JSON. Choose a setup saved by this browser app."); }
  return validateSetup(json);
}

function validateSetup(json) {
  if (!json || json.format !== format) {
    throw new Error("Choose an ARM browser setup file. Desktop study sessions are not supported in this first browser version.");
  }
  if (json.schemaVersion !== 1) {
    throw new Error("This setup uses a different version. Open it with the version of ARM that saved it.");
  }
  if (typeof json.name !== "string" || json.name.length > 80
    || typeof json.camera?.preferredLabel !== "string" || json.camera.preferredLabel.length > 256
    || typeof json.view?.mirrored !== "boolean"
    || typeof json.savedAt !== "string" || !Number.isFinite(Date.parse(json.savedAt))) {
    throw new Error("The setup is missing valid settings. Your current setup has not been changed.");
  }
  // Copy known fields, rather than mixing an imported object into live application state.
  return {
    format, schemaVersion: 1, savedAt: json.savedAt, name: json.name,
    camera: { preferredLabel: json.camera.preferredLabel },
    view: { mirrored: json.view.mirrored },
  };
}

export function setupFilename(setup) {
  const name = setup.name.normalize("NFC")
    .replace(/[^\p{L}\p{N}_-]+/gu, "_").replace(/^_+|_+$/g, "");
  // Limit bytes too: filenames on common filesystems have a 255-byte component limit.
  let prefix = "";
  for (const character of name) {
    if (new TextEncoder().encode(prefix + character).length > 120) break;
    prefix += character;
  }
  const timestamp = new Date(setup.savedAt).toISOString().replace(/[:.]/g, "-");
  return `${prefix || "ARM"}_setup_${timestamp}.json`;
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
