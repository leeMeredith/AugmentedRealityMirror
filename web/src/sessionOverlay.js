// Presentation only: native modal focus, dismissal and the camera's quick-score tray.
export class SessionOverlay {
  constructor(dialog) {
    const open = document.querySelector("#open-session");
    const quick = document.querySelector("#quick-pain-panel");
    const score = document.querySelector("#record-pain-view");
    const closeQuick = () => { quick.hidden = true; score.setAttribute("aria-expanded", "false"); };
    const show = () => {
      closeQuick();
      if (!dialog.open) dialog.showModal();
      dialog.querySelector("#session-content").scrollTop = 0;
    };
    open.addEventListener("click", show);
    document.querySelector("#end-session-view").addEventListener("click", show);
    dialog.querySelector("#close-session").addEventListener("click", () => dialog.close());
    dialog.addEventListener("session:started", () => dialog.close());
    dialog.addEventListener("close", () => open.focus({preventScroll:true}));
    // Clicking outside the card dismisses it; clicks in its padding stay inside.
    dialog.addEventListener("click", event => {
      const r = dialog.getBoundingClientRect();
      if (event.target === dialog && (event.clientX < r.left || event.clientX > r.right || event.clientY < r.top || event.clientY > r.bottom)) dialog.close();
    });
    score.addEventListener("click", () => {
      quick.hidden = !quick.hidden;
      score.setAttribute("aria-expanded", String(!quick.hidden));
      if (!quick.hidden) quick.querySelector("button").focus();
    });
    quick.addEventListener("click", event => { if (event.target.closest("button")) {closeQuick(); score.focus();} });
    document.addEventListener("click", event => {
      if (!quick.hidden && !quick.contains(event.target) && event.target !== score) closeQuick();
    });
    document.addEventListener("keydown", event => {
      if (event.key !== "Escape") return;
      if (dialog.open || !quick.hidden) {
        // Let the native dialog close without also exiting the camera's expanded view.
        event.stopImmediatePropagation();
        if (!quick.hidden) {event.preventDefault(); closeQuick(); score.focus();}
      }
    }, true);
  }
}
