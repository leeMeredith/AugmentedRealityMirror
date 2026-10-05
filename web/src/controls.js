// DOM details only. The application decides when to start a camera or change settings.
export class Controls {
  video = document.querySelector("#camera-video");
  canvas = document.querySelector("#mirror-canvas");
  start = document.querySelector("#start-camera");
  stop = document.querySelector("#stop-camera");
  cameras = document.querySelector("#camera-select");
  mirror = document.querySelector("#mirror-enabled");
  name = document.querySelector("#setup-name");
  save = document.querySelector("#save-setup");
  load = document.querySelector("#load-setup");
  file = document.querySelector("#setup-file");

  cameraState(state) {
    const live = state === "live";
    const pending = state === "pending";
    this.start.disabled = pending;
    this.start.textContent = live ? "Use selected camera" : "Start camera";
    this.stop.disabled = !live && !pending;
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
