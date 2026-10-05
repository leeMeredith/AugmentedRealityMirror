// Camera ownership only. Rendering, files, and page controls live elsewhere.
export class CaptureRGB {
  #video;
  #stream = null;
  #generation = 0;
  onEnded = () => {};

  constructor(video) { this.#video = video; }

  get active() { return this.#stream !== null; }

  get device() {
    const track = this.#stream?.getVideoTracks()[0];
    return { id: track?.getSettings().deviceId ?? "", label: track?.label ?? "" };
  }

  async start(deviceId = "") {
    this.stop();
    const generation = this.#generation;
    if (!globalThis.isSecureContext) {
      throw new Error("Open this app using its HTTPS link, or localhost when developing, to use the camera.");
    }
    if (!navigator.mediaDevices?.getUserMedia) {
      throw new Error("Camera access is unavailable in this browser. Try a current browser or ask whether camera access is restricted on this computer.");
    }

    let stream;
    try {
      stream = await navigator.mediaDevices.getUserMedia({
        audio: false,
        video: {
          width: { ideal: 640 }, height: { ideal: 480 }, frameRate: { ideal: 30 },
          ...(deviceId ? { deviceId: { exact: deviceId } } : {}),
        },
      });
      // Stop can be pressed while the browser permission prompt is still open.
      if (generation !== this.#generation) {
        stream.getTracks().forEach(track => track.stop());
        return false;
      }
      this.#stream = stream;
      this.#video.srcObject = stream;
      await this.#video.play();
      if (generation !== this.#generation) return false;
      const track = stream.getVideoTracks()[0];
      if (!track || track.readyState === "ended") {
        throw new Error("The camera stopped before it was ready. Reconnect it and try again.");
      }
      track.addEventListener("ended", () => {
        if (generation !== this.#generation) return;
        this.stop();
        this.onEnded();
      }, { once: true });
      return true;
    } catch (error) {
      stream?.getTracks().forEach(track => track.stop());
      if (generation !== this.#generation) return false;
      this.stop();
      throw new Error(cameraError(error));
    }
  }

  stop() {
    ++this.#generation;
    this.#stream?.getTracks().forEach(track => track.stop());
    this.#stream = null;
    this.#video.pause();
    this.#video.srcObject = null;
  }

  async list() {
    if (!navigator.mediaDevices?.enumerateDevices) return [];
    return (await navigator.mediaDevices.enumerateDevices())
      .filter(device => device.kind === "videoinput");
  }
}

export function cameraError(error) {
  const messages = {
    NotAllowedError: "Camera access was not allowed. Check this site's camera permission and your computer's privacy settings, then press Start camera. On a managed computer, IT may need to allow access.",
    SecurityError: "Camera access is blocked by browser or computer policy. Check permissions or contact the computer administrator.",
    NotFoundError: "No camera was found. Connect a camera and try again.",
    OverconstrainedError: "The selected camera is unavailable. Choose Default camera or another connected camera and try again.",
    NotReadableError: "The camera could not start. Close other apps using it, check its connection, and try again.",
    AbortError: "The camera request was interrupted. Try starting it again.",
  };
  return messages[error?.name] ?? error?.message ?? "The camera could not start. Check its connection and permissions, then try again.";
}
