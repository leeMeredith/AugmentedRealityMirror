// Quadrants use screen coordinates after the overall camera orientation.
export const REGION_IDS = ["topLeft", "topRight", "bottomLeft", "bottomRight"];
export const REGION_LABELS = ["Top left", "Top right", "Bottom left", "Bottom right"];
export const defaultRegion = () => ({ mirrorX: false });
export const defaultRegions = () => Object.fromEntries(REGION_IDS.map(id => [id, defaultRegion()]));
export const defaultCenter = () => ({ x: 50, y: 50 });

export function validateCenter(center) {
  if (!center || ![center.x, center.y].every(value => Number.isFinite(value) && value >= 10 && value <= 90)) {
    throw new Error("The setup has invalid alignment settings. Your current setup has not been changed.");
  }
  return { x: center.x, y: center.y };
}

export function validateRegions(regions) {
  return Object.fromEntries(REGION_IDS.map(id => {
    if (typeof regions?.[id]?.mirrorX !== "boolean") {
      throw new Error("The setup has invalid quadrant settings. Your current setup has not been changed.");
    }
    return [id, { mirrorX: regions[id].mirrorX }];
  }));
}

export const hasRegionAdjustments = regions => REGION_IDS.some(id => regions[id].mirrorX);

// Shared integer boundaries tile the frame exactly, including odd camera dimensions.
// These rectangles are masks over full images, not source crops to resize.
export function regionBounds(width, height, center, index) {
  const splitX = Math.max(1, Math.min(width - 1, Math.round(width * center.x / 100)));
  const splitY = Math.max(1, Math.min(height - 1, Math.round(height * center.y / 100)));
  const column = index % 2, row = Math.floor(index / 2);
  const x = column ? splitX : 0, y = row ? splitY : 0;
  const w = column ? width - splitX : splitX, h = row ? height - splitY : splitY;
  return { x, y, w, h };
}

export function drawRegions(context, source, width, height, mirrored, regions, center = defaultCenter()) {
  context.setTransform(1, 0, 0, 1, 0, 0);
  REGION_IDS.forEach((id, index) => {
    const p = regionBounds(width, height, center, index);
    const flipX = mirrored !== regions[id].mirrorX;
    context.save();
    context.beginPath();
    context.rect(p.x, p.y, p.w, p.h);
    context.clip();
    // The reflection axis stays at the full frame's center. Only the mask moves.
    context.setTransform(flipX ? -1 : 1, 0, 0, 1, flipX ? width : 0, 0);
    context.drawImage(source, 0, 0, width, height);
    context.restore();
  });
}
