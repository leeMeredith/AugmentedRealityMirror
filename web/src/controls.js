// DOM details only. The application decides when to start a camera or change settings.
import { REGION_IDS, REGION_LABELS } from "./mirrorRegions.js";
export class Controls {
  video = document.querySelector("#camera-video");
  canvas = document.querySelector("#mirror-canvas");
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
  mirrorY = document.querySelector("#region-mirror-y");
  offsetX = document.querySelector("#region-offset-x");
  offsetY = document.querySelector("#region-offset-y");
  resetRegion = document.querySelector("#reset-region");

  regionState(id, region) {
    this.quadrants.forEach(button => button.setAttribute("aria-pressed", String(button.dataset.region === id)));
    const label = REGION_LABELS[REGION_IDS.indexOf(id)];
    document.querySelector("#selected-region").textContent = label;
    this.mirrorX.checked = region.mirrorX;
    this.mirrorY.checked = region.mirrorY;
    for (const [axis, slider] of [["X", this.offsetX], ["Y", this.offsetY]]) {
      const value = region[`offset${axis}`];
      slider.value = value;
      const text = value === 0 ? "Centered" : `${Math.abs(value)}% ${axis === "X" ? (value < 0 ? "left" : "right") : (value < 0 ? "up" : "down")}`;
      document.querySelector(`#region-offset-${axis.toLowerCase()}-value`).textContent = text;
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
    document.querySelector("#placeholder").hidden = live;
    const badge = document.querySelector("#camera-state");
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
