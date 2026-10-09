import { defaultCenter, defaultRegions, drawRegions, hasRegionAdjustments } from "./mirrorRegions.js";
// Owns the canvas and draw loop; regional composition lives in mirrorRegions.
export class MirrorView {
  #canvas;
  #context;
  #video;
  #frame = null;
  mirrored = true;
  regions = defaultRegions();
  center = defaultCenter();

  constructor(canvas, video) {
    this.#canvas = canvas;
    this.#context = canvas.getContext("2d", { alpha: false });
    this.#video = video;
  }

  start() {
    this.stop();
    const draw = () => {
      const { videoWidth: width, videoHeight: height, readyState } = this.#video;
      if (width && height && readyState >= 2) {
        if (this.#canvas.width !== width || this.#canvas.height !== height) {
          this.#canvas.width = width;
          this.#canvas.height = height;
        }
        if (hasRegionAdjustments(this.regions)) {
          drawRegions(this.#context, this.#video, width, height, this.mirrored, this.regions, this.center);
        } else {
          this.#context.setTransform(this.mirrored ? -1 : 1, 0, 0, 1, this.mirrored ? width : 0, 0);
          this.#context.drawImage(this.#video, 0, 0, width, height);
        }
      }
      this.#frame = requestAnimationFrame(draw);
    };
    draw();
  }

  stop() {
    if (this.#frame !== null) cancelAnimationFrame(this.#frame);
    this.#frame = null;
    this.#context.setTransform(1, 0, 0, 1, 0, 0);
    this.#context.fillStyle = "#142f2c";
    this.#context.fillRect(0, 0, this.#canvas.width, this.#canvas.height);
  }
}
