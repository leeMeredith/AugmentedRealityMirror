// DOM details only. The application decides when to start a camera or change settings.
import { REGION_IDS, REGION_LABELS } from "./mirrorRegions.js";
export class Controls {
  video = document.querySelector("#camera-video");
  canvas = document.querySelector("#mirror-canvas");
  preview = document.querySelector(".preview-panel");
  start = document.querySelector("#start-camera");
  stop = document.querySelector("#stop-camera");
  stopView = document.querySelector("#stop-view");
  cameras = document.querySelector("#camera-select");
  reverse = document.querySelector("#reverse-view");
  name = document.querySelector("#setup-name");
  save = document.querySelector("#save-setup");
  load = document.querySelector("#load-setup");
  file = document.querySelector("#setup-file");
  build = document.querySelector("#build-version");
  quadrants = [...document.querySelectorAll("[data-region]")];
  mirrorX = document.querySelector("#region-mirror-x");
  centerX = document.querySelector("#center-x");
  centerY = document.querySelector("#center-y");
  resetAlignment = document.querySelector("#reset-alignment");

  regionState(id, region) {
    this.quadrants.forEach(button => button.setAttribute("aria-pressed", String(button.dataset.region === id)));
    const label = REGION_LABELS[REGION_IDS.indexOf(id)];
    document.querySelector("#selected-region").textContent = label;
    this.mirrorX.checked = region.mirrorX;
  }

  alignmentState(center) {
    for (const [axis, slider] of [["x", this.centerX], ["y", this.centerY]]) {
      const value = center[axis];
      slider.value = value;
      const text = value === 50 ? "Centered (50%)" : `${value}% from ${axis === "x" ? "left" : "top"}`;
      document.querySelector(`#center-${axis}-value`).textContent = text;
      slider.setAttribute("aria-valuetext", text);
    }
  }

  cameraState(state) {
    const live = state === "live";
    const pending = state === "pending";
    this.start.disabled = pending;
    this.start.textContent = live ? "Use selected camera" : "Start camera";
    this.stop.disabled = !live && !pending;
    this.stopView.disabled = !live && !pending;
    this.cameras.disabled = pending;
    this.canvas.hidden = !live;
    this.preview.querySelector("#placeholder").hidden = live;
    const badge = this.preview.querySelector("#camera-state");
    badge.textContent = live ? "Camera on" : pending ? "Waiting for camera" : "Camera off";
    badge.dataset.live = String(live);
  }

  cameraStatus(message, error = false) { this.#message("#camera-status", message, error); }
  setupStatus(message, error = false) { this.#message("#setup-status", message, error); }

  #message(selector, message, error) {
    const element = document.querySelector(selector);
    element.textContent = message;
    element.classList.toggle("error", error);
  }

  cameraOptions(devices, selectedId) {
    this.cameras.replaceChildren(new Option("Default camera", ""));
    devices.forEach((device, index) => {
      if (device.deviceId) this.cameras.add(new Option(device.label || `Camera ${index + 1}`, device.deviceId));
    });
    if (devices.some(device => device.deviceId === selectedId)) this.cameras.value = selectedId;
  }
}
