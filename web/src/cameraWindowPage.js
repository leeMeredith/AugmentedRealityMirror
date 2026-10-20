// This page owns fullscreen only. Capture, settings and session data stay in the opener.
const root = document.querySelector("#camera-window-root");
const button = document.querySelector("#camera-window-fullscreen");
const status = document.querySelector("#camera-window-status");
button.addEventListener("click", async () => {
  try {
    if (document.fullscreenElement) await document.exitFullscreen();
    else if (root.requestFullscreen && document.fullscreenEnabled !== false) await root.requestFullscreen();
    else status.textContent = "Fullscreen is unavailable here. You can resize this camera window.";
  } catch { status.textContent = "Fullscreen is unavailable here. You can resize this camera window."; }
});
document.addEventListener("keydown", event => {
  if (event.key === "Escape" && document.fullscreenElement) {
    event.preventDefault();
    document.exitFullscreen().catch(() => { status.textContent = "Use the browser's fullscreen exit control to return."; });
  }
});
document.addEventListener("fullscreenchange", () => {
  button.textContent = document.fullscreenElement ? "Exit fullscreen" : "Fullscreen";
});
let connected = false;
try { connected = Boolean(window.opener?.armCameraWindow?.connect(window)); }
catch { /* A standalone or restricted window has no controller. */ }
if (!connected) {
  status.textContent = "Open this camera view from the main ARM application.";
  document.querySelector("#camera-window-help").hidden = false;
}
