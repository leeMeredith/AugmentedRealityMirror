// This page owns fullscreen only. Capture, settings and session data stay in the opener.
const root = document.querySelector("#camera-window-root");
const button = document.querySelector("#camera-window-fullscreen");
const status = document.querySelector("#camera-window-status");
const message = text => { status.textContent = text; status.hidden = false; };
const toggle = document.querySelector("#camera-window-toggle");
const score = document.querySelector("#camera-window-score");
const quick = document.querySelector("#camera-quick-panel");
const closeQuick = () => {quick.hidden = true; score.setAttribute("aria-expanded", "false");};
toggle.addEventListener("click", () => {
  const hidden = root.classList.toggle("camera-toolbar-hidden");
  toggle.textContent = hidden ? "Show controls" : "Hide controls";
  toggle.setAttribute("aria-expanded", String(!hidden));
  closeQuick();
});
score.addEventListener("click", () => {
  quick.hidden = !quick.hidden;
  score.setAttribute("aria-expanded", String(!quick.hidden));
  if (!quick.hidden) quick.querySelector("button").focus();
});
quick.addEventListener("click", event => {if(event.target.closest("button")){closeQuick();score.focus();}});
document.addEventListener("click", event => {if(!quick.hidden && !quick.contains(event.target) && event.target !== score) closeQuick();});
button.addEventListener("click", async () => {
  try {
    if (document.fullscreenElement) await document.exitFullscreen();
    else if (root.requestFullscreen && document.fullscreenEnabled !== false) await root.requestFullscreen();
    else message("Fullscreen is unavailable here. You can resize this camera window.");
  } catch { message("Fullscreen is unavailable here. You can resize this camera window."); }
});
document.addEventListener("keydown", event => {
  if (event.key === "Escape" && !quick.hidden) {event.preventDefault();closeQuick();score.focus();return;}
  if (event.key === "Escape" && document.fullscreenElement) {
    event.preventDefault();
    document.exitFullscreen().catch(() => { message("Use the browser's fullscreen exit control to return."); });
  }
});
document.addEventListener("fullscreenchange", () => {
  button.textContent = document.fullscreenElement ? "Exit fullscreen" : "Fullscreen";
});
let connected = false;
try { connected = Boolean(window.opener?.armCameraWindow?.connect(window)); }
catch { /* A standalone or restricted window has no controller. */ }
if (!connected) {
  message("Open this camera view from the main ARM application.");
  document.querySelector("#camera-window-help").hidden = false;
}
