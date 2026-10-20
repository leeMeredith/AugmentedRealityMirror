// One capture and one canvas. Move that canvas between documents without duplicating video.
export class CameraWindow {
  #child = null;
  #windowName = "ARM-camera-" + Math.random().toString(36).slice(2);
  #view;
  #controls;
  #sessions;
  #tabs;
  #fullscreen;
  #stage;
  #open;
  #return;
  #notice;
  #observer = null;
  #detachKeyboard = null;
  #exitReady = Promise.resolve();

  constructor({view, controls, sessions, tabs, fullscreen}) {
    this.#view = view; this.#controls = controls; this.#sessions = sessions;
    this.#tabs = tabs; this.#fullscreen = fullscreen;
    this.#stage = controls.canvas.parentElement;
    this.#open = document.querySelector("#open-camera-window");
    this.#return = document.querySelector("#return-camera-window");
    this.#notice = document.querySelector("#camera-window-notice");
    window.armCameraWindow = {connect: child => this.#connect(child)};
    this.#open.addEventListener("click", () => this.open());
    this.#return.addEventListener("click", () => this.restore());
    window.addEventListener("pagehide", () => this.restore(false));
  }

  open() {
    if (this.#child && !this.#child.closed) { this.#child.focus(); return; }
    const child = window.open("camera.html", this.#windowName, "popup,width=1000,height=760");
    if (!child) {
      this.#notice.hidden = false;
      this.#notice.textContent = "The browser blocked the camera window. Allow pop-ups for this site and retry, or keep using the view here.";
      return;
    }
    this.#child = child;
    this.#exitReady = this.#fullscreen.exit();
  }

  #connect(child) {
    if (child !== this.#child || child.closed) return false;
    const doc = child.document;
    this.#view.moveCanvas(doc.querySelector("#camera-window-stage"));
    this.#controls.preview.hidden = true;
    document.querySelector("#mirror-workspace").classList.add("camera-detached");
    this.#open.textContent = "Show camera window";
    this.#return.hidden = false;
    this.#notice.hidden = false;
    this.#notice.textContent = "Camera view is in its own window. Choose View settings or Session below.";
    document.querySelector("#fullscreen-view").disabled = true;
    document.querySelector("#toggle-controls").disabled = true;
    this.#tabs.select("session-tab");
    this.#fullscreen.showControls();
    this.#exitReady.then(() => { if (this.#child === child) this.#fullscreen.showControls(); });
    this.#detachKeyboard = this.#sessions.attachKeyboard(doc);
    doc.querySelector("#camera-window-stop").addEventListener("click", () => this.#controls.stopView.click());
    doc.querySelector("#camera-window-end").addEventListener("click", () => document.querySelector("#end-session-view").click());
    const returnButton = doc.querySelector("#camera-window-return");
    returnButton.disabled = false;
    returnButton.addEventListener("click", () => this.restore());
    child.addEventListener("pagehide", () => { if (this.#child === child) this.restore(false); });
    const sync = () => {
      if (child.closed || this.#child !== child) return;
      doc.querySelector("#camera-window-summary").textContent = document.querySelector("#session-summary").textContent;
      doc.querySelector("#camera-window-end").disabled = document.querySelector("#end-session-view").disabled;
      doc.querySelector("#camera-window-stop").disabled = this.#controls.stopView.disabled;
      const badge = this.#controls.preview.querySelector("#camera-state");
      doc.querySelector("#camera-window-state").textContent = badge.textContent;
      doc.querySelector("#camera-window-state").dataset.live = badge.dataset.live;
      doc.querySelector("#camera-window-placeholder").hidden = !this.#controls.canvas.hidden;
    };
    this.#observer = new MutationObserver(sync);
    this.#observer.observe(document.querySelector("#mirror-workspace"), {attributes:true, childList:true, subtree:true});
    doc.querySelector("#camera-window-status").textContent = "Use number keys to score here or in the main window. The main window keeps session records; leave it open.";
    sync();
    child.focus();
    return true;
  }

  restore(focus = true) {
    const child = this.#child;
    this.#child = null;
    this.#observer?.disconnect(); this.#observer = null;
    this.#detachKeyboard?.(); this.#detachKeyboard = null;
    if (this.#controls.canvas.parentElement !== this.#stage) this.#view.moveCanvas(this.#stage);
    this.#controls.preview.hidden = false;
    document.querySelector("#mirror-workspace").classList.remove("camera-detached");
    this.#open.textContent = "Open camera window";
    this.#return.hidden = true;
    this.#notice.hidden = true;
    document.querySelector("#fullscreen-view").disabled = false;
    document.querySelector("#toggle-controls").disabled = false;
    if (child && !child.closed) child.close();
    if (focus) this.#open.focus();
  }
}
