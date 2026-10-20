import { StudySession, downloadSession, sessionFilename } from "./studySession.js";

// Owns only the optional session form and scoring actions, including keyboard shortcuts.
export class SessionControls {
  #session = new StudySession();
  #root;
  #viewSettings;
  #beforeScore = null;
  #ending = false;
  #name;
  #details;
  #start;
  #end;
  #endView;
  #save;
  #next;
  #scores;
  #phase;
  #status;
  #summary;
  #recent;

  constructor(root, viewSettings) {
    this.#root = root;
    this.#viewSettings = viewSettings;
    const find = selector => root.querySelector(selector);
    this.#name = document.querySelector("#participant-name");
    this.#details = find("#participant-details");
    this.#start = find("#start-session");
    this.#end = find("#end-session");
    this.#endView = document.querySelector("#end-session-view");
    this.#save = find("#save-session");
    this.#next = find("#new-session");
    this.#scores = find("#pain-scores");
    this.#phase = find("#pain-phase");
    this.#status = find("#session-status");
    this.#summary = document.querySelector("#session-summary");
    this.#recent = find("#pain-history");
    for (let score = 0; score <= 10; score++) {
      const button = document.createElement("button");
      button.type = "button";
      button.dataset.score = String(score);
      button.textContent = score === 0 ? "No pain (0)" : String(score);
      button.setAttribute("aria-label", score === 0 ? "No pain, score 0" : `Pain score ${score}`);
      button.addEventListener("click", () => this.#score(score));
      this.#scores.append(button);
    }
    this.#start.addEventListener("click", () => this.#act(() => {
      const limbLoss = [...root.querySelectorAll("[data-limb]")].filter(input => input.checked).map(input => ({
        side: input.dataset.side, limb: input.dataset.limb,
        level: find(`#${input.id}-level`).value,
      }));
      this.#session.start({ participantName: this.#name.value, limbLoss, painScore: this.#beforeScore, viewSettings: this.#viewSettings() });
      this.#message("Session started. Each score selection records immediately. Press End session when finished.");
      this.#refresh();
      // Leave name/text inputs before making number keys available.
      this.#start.blur();
      this.#root.scrollTop = 0;
    }));
    const end = () => {
      if (!this.#session.active || this.#ending) return;
      this.#ending = true;
      this.#message("Choose the final pain score. This will end the session and request its JSON download.");
      this.#refresh();
    };
    this.#end.addEventListener("click", end);
    this.#endView.addEventListener("click", end);
    find("#continue-session").addEventListener("click", () => {
      this.#ending = false;
      this.#message("Session continues. Score selections now record during-use pain.");
      this.#refresh();
    });
    this.#save.addEventListener("click", () => this.#act(() => this.#download()));
    this.#next.addEventListener("click", () => {
      this.#session.clear();
      this.#beforeScore = null;
      this.#ending = false;
      this.#message("Ready for a new session. Check the participant details and choose a starting score.");
      this.#refresh();
    });
    this.attachKeyboard(document);
    window.addEventListener("beforeunload", event => {
      if (!this.#session.active) return;
      event.preventDefault();
      event.returnValue = "";
    });
    this.#refresh();
  }

  get canChangeParticipantName() { return this.#session.record === null; }

  attachKeyboard(target) {
    target.addEventListener("keydown", this.#key);
    return () => target.removeEventListener("keydown", this.#key);
  }

  #key = event => {
      if (event.repeat || event.isComposing || event.ctrlKey || event.altKey || event.metaKey
        || !/^[0-9]$/.test(event.key)
        || event.target.closest?.("input, select, textarea, [contenteditable]:not([contenteditable='false'])")) return;
      if (this.#session.record?.status === "completed") return;
      event.preventDefault();
      this.#score(event.key === "0" ? 10 : Number(event.key));
  };

  #act(action) {
    try { action(); }
    catch (error) { this.#message(error.message, true); }
    finally { this.#refresh(); }
  }

  #score(score) {
    this.#act(() => {
      if (!this.#session.record) {
        this.#beforeScore = score;
        this.#message(`Starting pain score: ${score}. Press Start session when ready.`);
      } else if (this.#session.active) {
        const entry = this.#session.addScore(score, this.#viewSettings(), this.#ending ? "after" : "during");
        this.#message(`${entry.phase === "after" ? "Final" : "During-use"} pain score ${score} recorded at ${entry.elapsedSeconds} seconds.`);
        if (this.#ending) {
          this.#ending = false;
          this.#download();
        }
      }
      this.#refresh();
    });
  }

  #download() {
    const record = this.#session.record;
    downloadSession(record);
    this.#message(`Download requested: ${sessionFilename(record)}. Check Downloads or your browser's save prompt. ${record.status === "active" ? "This is an unfinished session; recording continues." : "Session complete. You can download it again before starting a new session."}`);
  }

  #message(text, error = false) {
    this.#status.textContent = text;
    this.#status.classList.toggle("error", error);
  }

  #refresh() {
    const record = this.#session.record;
    const active = this.#session.active;
    const completed = record?.status === "completed";
    this.#details.disabled = active || completed;
    this.#details.hidden = Boolean(record);
    const limbSummary = this.#root.querySelector("#limb-summary");
    limbSummary.hidden = !record;
    if (record) limbSummary.textContent = "Limb loss: " + (record.limbLoss.map(({side, limb, level}) =>
      `${side} ${limb}${level ? ` (${level})` : ""}`).join(", ") || "not specified") + ". Details are fixed for this session.";
    this.#name.disabled = active || completed;
    this.#start.disabled = active || completed || this.#beforeScore === null;
    this.#end.disabled = !active || this.#ending;
    this.#endView.disabled = !active || this.#ending;
    this.#save.disabled = !record;
    this.#next.hidden = !completed;
    this.#root.querySelector("#continue-session").hidden = !this.#ending;
    this.#phase.textContent = completed ? "Session complete" : this.#ending ? "Pain at end of use" : active ? "Pain during use" : "Pain before use";
    const latest = record?.painEntries.at(-1);
    this.#summary.textContent = completed ? `Session ended · final pain ${latest.painScore}/10`
      : this.#ending ? "Session: select the final pain score (1–9; key 0 = 10)."
      : active ? `Session active · last pain ${latest.painScore}/10 · keys 1–9; key 0 = 10`
      : "Optional pain session: choose a starting score in the controls.";
    this.#scores.querySelectorAll("button").forEach(button => {
      button.disabled = completed;
      // During-use scores are actions, not a persistent selection.
      if (!record) button.setAttribute("aria-pressed", String(Number(button.dataset.score) === this.#beforeScore));
      else button.removeAttribute("aria-pressed");
    });
    this.#recent.replaceChildren();
    for (const entry of (record?.painEntries ?? []).slice(-5)) {
      const item = document.createElement("li");
      item.textContent = `${entry.phase} · ${entry.painScore}/10 · ${entry.elapsedSeconds}s · ${new Date(entry.recordedAt).toLocaleTimeString()}`;
      this.#recent.append(item);
    }
  }
}
