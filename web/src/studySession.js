// Session records only: no DOM, camera ownership, browser storage or chart library.
const copy = value => structuredClone(value);
const validScore = value => Number.isInteger(value) && value >= 0 && value <= 10;

export class StudySession {
  #record = null;
  #clock;
  #now;
  #id;
  #startedTick = 0;
  #elapsed = 0;

  constructor({ now = () => new Date(), clock = () => performance.now(), id = () => crypto.randomUUID() } = {}) {
    this.#now = now;
    this.#clock = clock;
    this.#id = id;
  }

  get active() { return this.#record?.status === "active"; }
  get record() { return this.#record ? copy(this.#record) : null; }

  start({ participantName = "", limbLoss = [], painScore, viewSettings }) {
    if (this.active) throw new Error("End the current session before starting another.");
    if (!validScore(painScore)) throw new Error("Choose a pain score from 0 to 10 before starting.");
    if (typeof participantName !== "string" || participantName.trim().length > 80) throw new Error("Use a participant name of at most 80 characters.");
    if (!Array.isArray(limbLoss) || limbLoss.length > 4) throw new Error("Choose up to four limb-loss locations.");
    const seen = new Set();
    const limbs = limbLoss.map(limb => {
      if (!limb || !["right", "left"].includes(limb.side) || !["arm", "leg"].includes(limb.limb)
        || typeof limb.level !== "string" || limb.level.length > 80 || seen.has(`${limb.side}-${limb.limb}`)) {
        throw new Error("Check the selected limb-loss locations and descriptions.");
      }
      seen.add(`${limb.side}-${limb.limb}`);
      return { side: limb.side, limb: limb.limb, level: limb.level.trim() };
    });
    const settings = copy(viewSettings);
    const date = this.#now();
    this.#startedTick = this.#clock();
    this.#elapsed = 0;
    this.#record = {
      format: "arm-rgb-study-session", schemaVersion: 1, sessionId: this.#id(),
      participantName: participantName.trim(), limbLoss: limbs,
      startedAt: date.toISOString(), endedAt: null, status: "active",
      timeZone: Intl.DateTimeFormat().resolvedOptions().timeZone,
      viewSettings: settings, endViewSettings: null,
      painEntries: [{ phase: "before", recordedAt: date.toISOString(), elapsedSeconds: 0, painScore, viewSettings: copy(settings) }],
    };
    return this.record;
  }

  addScore(painScore, viewSettings, phase = "during") {
    if (!this.active) throw new Error("Start a session before recording pain.");
    if (!validScore(painScore)) throw new Error("Choose a pain score from 0 to 10.");
    if (!["during", "after"].includes(phase)) throw new Error("Choose a during or end-of-session score.");
    const settings = copy(viewSettings);
    const recordedAt = this.#now().toISOString();
    this.#elapsed = Math.max(this.#elapsed, Math.round((this.#clock() - this.#startedTick) / 10) / 100, 0);
    const entry = { phase, recordedAt, elapsedSeconds: this.#elapsed, painScore, viewSettings: settings };
    this.#record.painEntries.push(entry);
    if (phase === "after") {
      this.#record.endedAt = recordedAt;
      this.#record.endViewSettings = copy(settings);
      this.#record.status = "completed";
    }
    return copy(entry);
  }

  clear() {
    if (this.active) throw new Error("End the current session before starting another.");
    this.#record = null;
  }
}

export function sessionFilename(record) {
  const safe = record.participantName.normalize("NFC").replace(/[^\p{L}\p{N}_-]+/gu, "_").replace(/^_+|_+$/g, "");
  let prefix = "";
  for (const character of safe) {
    if (new TextEncoder().encode(prefix + character).length > 120) break;
    prefix += character;
  }
  // UTC date/time includes milliseconds; the full session ID remains inside the JSON.
  const timestamp = new Date(record.startedAt).toISOString().replace(/[:.]/g, "-");
  return `${prefix ? prefix + "_" : ""}${timestamp}.json`;
}

export function downloadSession(record) {
  if (!record) throw new Error("Start a session before saving it.");
  const url = URL.createObjectURL(new Blob([JSON.stringify(record, null, 2) + "\n"], { type: "application/json" }));
  const link = document.createElement("a");
  link.href = url;
  link.download = sessionFilename(record);
  document.body.append(link);
  link.click();
  link.remove();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
}
