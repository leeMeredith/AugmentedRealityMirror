// Two local panels; selecting a tab never changes camera or session state.
export class ControlTabs {
  #buttons;
  constructor(root) {
    this.#buttons = [...root.querySelectorAll('[role="tab"]')];
    for (const button of this.#buttons) {
      button.addEventListener("click", () => this.select(button.id));
      button.addEventListener("keydown", event => {
        if (!["ArrowLeft", "ArrowRight", "Home", "End"].includes(event.key)) return;
        event.preventDefault();
        const index = this.#buttons.indexOf(button);
        const next = event.key === "Home" ? 0 : event.key === "End" ? this.#buttons.length - 1
          : (index + (event.key === "ArrowRight" ? 1 : -1) + this.#buttons.length) % this.#buttons.length;
        this.select(this.#buttons[next].id);
        this.#buttons[next].focus();
      });
    }
    this.select(this.#buttons[0].id);
  }
  select(id) {
    for (const button of this.#buttons) {
      const selected = button.id === id;
      button.setAttribute("aria-selected", String(selected));
      button.tabIndex = selected ? 0 : -1;
      document.getElementById(button.getAttribute("aria-controls")).hidden = !selected;
    }
  }
}
