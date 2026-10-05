// Display layout only. Fullscreen never changes the camera, image orientation, or saved setup.
export class FullscreenView {
  #root;
  #panel;
  #fullscreenButton;
  #controlsButton;
  #status;
  #expanded = false;
  #native = false;
  #busy = false;
  #hidden = false;
  #previousHidden = false;
  #outside = [];

  constructor(root) {
    this.#root = root;
    this.#panel = root.querySelector("#controls");
    this.#fullscreenButton = root.querySelector("#fullscreen-view");
    this.#controlsButton = root.querySelector("#toggle-controls");
    this.#status = root.querySelector("#fullscreen-status");
    this.#fullscreenButton.addEventListener("click", () => this.toggle());
    this.#controlsButton.addEventListener("click", () => this.#hideControls(!this.#hidden));
    document.addEventListener("fullscreenchange", () => {
      if (document.fullscreenElement === this.#root) {
        this.#native = true;
        if (!this.#expanded) {
          // Escape may have cancelled while the browser's request was pending.
          document.exitFullscreen().catch(() => this.#message("Use Escape to exit fullscreen."));
        } else {
          this.#fullscreenButton.textContent = "Exit fullscreen";
          this.#message("Press Escape or Exit fullscreen to return.");
        }
      } else if (this.#native) {
        this.#restore();
      }
    });
    document.addEventListener("keydown", event => {
      if (event.key === "Escape" && this.#expanded) {
        event.preventDefault();
        this.exit();
      }
    });
    window.addEventListener("pagehide", () => this.#restore(false));
  }

  async toggle() {
    if (this.#busy) return;
    if (this.#expanded) return this.exit();
    this.#previousHidden = this.#hidden;
    this.#expanded = true;
    this.#root.classList.add("is-expanded");
    document.body.classList.add("view-expanded");
    this.#outside = [...document.querySelectorAll(".masthead, .intro, footer, .skip-link")]
      .map(element => ({ element, inert: element.inert }));
    this.#outside.forEach(({ element }) => { element.inert = true; });
    this.#hideControls(true);
    this.#fullscreenButton.textContent = "Exit expanded view";
    this.#message("Expanded view. Press Escape or Exit expanded view to return.");

    if (this.#root.requestFullscreen && document.fullscreenEnabled !== false) {
      this.#busy = true;
      this.#fullscreenButton.disabled = true;
      try {
        await this.#root.requestFullscreen();
        // The request can resolve before fullscreenchange is dispatched.
        this.#native = document.fullscreenElement === this.#root;
      } catch {
        if (this.#expanded) this.#message("Fullscreen is unavailable here. Expanded view is on; press Escape or Exit expanded view to return.");
      } finally {
        this.#busy = false;
        this.#fullscreenButton.disabled = false;
        this.#fullscreenButton.focus({ preventScroll: true });
      }
    }
  }

  async exit() {
    if (document.fullscreenElement === this.#root) {
      this.#busy = true;
      this.#fullscreenButton.disabled = true;
      try { await document.exitFullscreen(); }
      catch {
        this.#message("The browser could not exit fullscreen. Use Escape to return.");
        return;
      } finally {
        this.#busy = false;
        this.#fullscreenButton.disabled = false;
        this.#fullscreenButton.focus({ preventScroll: true });
      }
    }
    this.#restore();
  }

  #hideControls(hidden) {
    this.#hidden = hidden;
    this.#panel.hidden = hidden;
    this.#root.classList.toggle("controls-hidden", hidden);
    this.#controlsButton.textContent = hidden ? "Show controls" : "Hide controls";
    this.#controlsButton.setAttribute("aria-expanded", String(!hidden));
  }

  #message(text) {
    this.#status.textContent = text;
    this.#status.hidden = false;
  }

  #restore(focus = true) {
    if (!this.#expanded && !this.#native) return;
    this.#expanded = false;
    this.#native = false;
    this.#root.classList.remove("is-expanded");
    document.body.classList.remove("view-expanded");
    this.#outside.forEach(({ element, inert }) => { element.inert = inert; });
    this.#outside = [];
    this.#hideControls(this.#previousHidden);
    this.#fullscreenButton.textContent = "Fullscreen";
    this.#status.hidden = true;
    if (focus) this.#fullscreenButton.focus({ preventScroll: true });
  }
}
