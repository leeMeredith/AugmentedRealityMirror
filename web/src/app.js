import { SessionOverlay } from "./sessionOverlay.js?v=12";
import { CameraWindow } from "./cameraWindow.js?v=12";
import { CaptureRGB } from "./captureRGB.js?v=12";
import { MirrorView } from "./mirrorView.js?v=12";
import { Controls } from "./controls.js?v=12";
import { SessionControls } from "./sessionControls.js?v=12";
import { FullscreenView } from "./fullscreenView.js?v=12";
import { defaultCenter } from "./mirrorRegions.js?v=12";
import { MAX_SETUP_BYTES, createSetup, parseSetup, downloadSetup, setupFilename } from "./setupFile.js?v=12";

const controls = new Controls();
const camera = new CaptureRGB(controls.video);
const view = new MirrorView(controls.canvas, controls.video);
const participantName = document.querySelector("#participant-name");
const fullscreen = new FullscreenView(document.querySelector("#mirror-workspace"));
// Set by the running JavaScript, so a stale HTML label alone cannot claim this build loaded.
controls.build.textContent = "Browser build 12";
let selectedRegion = "topLeft";
const refreshRegion = () => controls.regionState(selectedRegion, view.regions[selectedRegion]);
controls.quadrants.forEach(button => button.addEventListener("click", () => {
  selectedRegion = button.dataset.region;
  refreshRegion();
}));
controls.mirrorX.addEventListener("input", () => {
  view.regions[selectedRegion].mirrorX = controls.mirrorX.checked;
});
for (const [element, axis] of [[controls.centerX, "x"], [controls.centerY, "y"]]) {
  element.addEventListener("input", () => {
    view.center[axis] = Number(element.value);
    controls.alignmentState(view.center);
  });
}
controls.resetAlignment.addEventListener("click", () => {
  view.center = defaultCenter();
  controls.alignmentState(view.center);
});
refreshRegion();
controls.alignmentState(view.center);
let preferredCameraLabel = "";
let devices = [];
let request = 0;
let deviceListRequest = 0;
let fileRequest = 0;

async function refreshCameras() {
  const current = ++deviceListRequest;
  const listed = await camera.list();
  if (current !== deviceListRequest) return;
  devices = listed;
  const preferred = preferredCameraLabel
    ? devices.filter(device => device.label === preferredCameraLabel) : [];
  const selectedId = preferred.length === 1 ? preferred[0].deviceId
    : controls.cameras.value || camera.device.id;
  controls.cameraOptions(devices, selectedId);
}

function stopCamera(message = "Camera stopped. Press Start camera when you are ready.") {
  ++request;
  camera.stop();
  view.stop();
  controls.cameraState("off");
  controls.cameraStatus(message);
}

async function startCamera() {
  const current = ++request;
  view.stop();
  controls.cameraState("pending");
  controls.cameraStatus("Allow camera access in your browser. You can press Stop to cancel here.");
  try {
    const started = await camera.start(controls.cameras.value);
    if (!started || current !== request) return;
    view.start();
    controls.cameraState("live");
    controls.cameraStatus("Camera is on. Raise a hand to check your view. Choose another camera and press Use selected camera to switch.");
    try {
      await refreshCameras();
      if (current !== request) return;
      if (preferredCameraLabel && camera.device.label !== preferredCameraLabel) {
        const matches = devices.filter(device => device.label === preferredCameraLabel);
        controls.cameraStatus(matches.length === 1
          ? "Your saved camera is selected. Press Use selected camera to switch to it."
          : "Your saved camera could not be identified uniquely. Confirm the camera in the list before continuing.");
      }
    } catch {
      if (current === request) controls.cameraStatus("Camera is on, but the camera list could not be refreshed. You can still use this view.");
    }
  } catch (error) {
    if (current !== request) return;
    controls.cameraState("off");
    controls.cameraStatus(error.message, true);
  }
}

controls.start.addEventListener("click", startCamera);
controls.stop.addEventListener("click", () => stopCamera());
controls.stopView.addEventListener("click", () => stopCamera());
controls.cameras.addEventListener("change", () => {
  preferredCameraLabel = devices.find(device => device.deviceId === controls.cameras.value)?.label ?? "";
  if (camera.active) controls.cameraStatus("Camera choice changed. Press Use selected camera to switch the live view.");
});
controls.reverse.addEventListener("click", () => { view.mirrored = !view.mirrored; });

controls.save.addEventListener("click", () => {
  try {
    const cameraLabel = controls.cameras.value
      ? devices.find(device => device.deviceId === controls.cameras.value)?.label ?? ""
      : preferredCameraLabel;
    const setup = createSetup({ name: controls.name.value, participantName: participantName.value, mirrored: view.mirrored, cameraLabel, regions: view.regions, center: view.center });
    downloadSetup(setup);
    controls.setupStatus(`Download requested: ${setupFilename(setup)}. Check your browser's downloads or save prompt.`);
  } catch (error) { controls.setupStatus(error.message, true); }
});

controls.load.addEventListener("click", () => controls.file.click());
controls.file.addEventListener("change", async () => {
  const file = controls.file.files[0];
  controls.file.value = "";
  if (!file) return;
  const current = ++fileRequest;
  try {
    if (file.size > MAX_SETUP_BYTES) throw new Error("This file is too large for a setup. Choose an ARM browser setup JSON file.");
    const setup = parseSetup(await file.text());
    if (current !== fileRequest) return;
    const nameKept = !sessionControls.canChangeParticipantName && participantName.value.trim() !== setup.participantName;
    if (sessionControls.canChangeParticipantName) participantName.value = setup.participantName;
    controls.name.value = setup.name;
    view.mirrored = setup.view.mirrored;
    view.regions = setup.view.regions;
    view.center = setup.view.center;
    controls.alignmentState(view.center);
    refreshRegion();
    preferredCameraLabel = setup.camera.preferredLabel;
    controls.cameras.value = "";
    try { await refreshCameras(); } catch { /* The user can still load mirror settings without camera permission. */ }
    if (current !== fileRequest) return;
    controls.setupStatus((setup.importNotice || "Setup loaded. Mirror choices and shared alignment restored.")
      + (nameKept ? " Current session name kept. Choose New session before loading a different participant." : " Participant name restored. Confirm the camera selection before use."));
    controls.cameraStatus(camera.active
      ? "Setup loaded; the current camera is still running. Press Use selected camera if you want to switch."
      : "Setup loaded. Press Start camera when you are ready.");
  } catch (error) {
    if (current === fileRequest) controls.setupStatus(error.message, true);
  }
});

camera.onEnded = () => stopCamera("The camera disconnected or stopped. Check its connection, select a camera, and try again.");
navigator.mediaDevices?.addEventListener("devicechange", () => { refreshCameras().catch(() => {}); });
// Returning from a suspended/back-forward page requires an explicit new Start.
window.addEventListener("pagehide", () => stopCamera());
controls.cameraState("off");
// No camera request at page load. Enumeration may return anonymous names until permission is granted.
refreshCameras().catch(() => {});

const sessionControls = new SessionControls(document.querySelector("#study-session"), () => ({
  ...createSetup({ mirrored: view.mirrored, regions: view.regions, center: view.center }).view,
  camera: { preferredLabel: camera.device.label || preferredCameraLabel },
}));

const sessionOverlay = new SessionOverlay(document.querySelector("#study-session"));

const cameraWindow = new CameraWindow({view, controls, sessions: sessionControls, fullscreen});
