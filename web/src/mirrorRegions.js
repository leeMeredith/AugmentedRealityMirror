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
// A reflected source is cropped uniformly to cover its receiving pane, never stretched.
export function regionDrawPlan(width, height, center, index, mirrorX, mirrored) {
  const splitX = Math.max(1, Math.min(width - 1, Math.round(width * center.x / 100)));
  const splitY = Math.max(1, Math.min(height - 1, Math.round(height * center.y / 100)));
  const column = index % 2, row = Math.floor(index / 2);
  const x = column ? splitX : 0, y = row ? splitY : 0;
  const w = column ? width - splitX : splitX, h = row ? height - splitY : splitY;
  const sourceColumn = mirrorX ? 1 - column : column;
  const sourceX = sourceColumn ? splitX : 0;
  const sourceW = sourceColumn ? width - splitX : splitX;
  const scale = Math.max(w / sourceW, 1);
  const sw = w / scale, sh = h / scale;
  // Keep the crop against the shared vertical boundary and centered in its row.
  const sx = sourceColumn ? sourceX : sourceX + sourceW - sw;
  return { x, y, w, h, sx: mirrored ? width - sx - sw : sx,
    sy: y + (h - sh) / 2, sw, sh, flipX: mirrored !== mirrorX };
}

export function drawRegions(context, source, width, height, mirrored, regions, center = defaultCenter()) {
  context.setTransform(1, 0, 0, 1, 0, 0);
  REGION_IDS.forEach((id, index) => {
    const p = regionDrawPlan(width, height, center, index, regions[id].mirrorX, mirrored);
    context.save();
    context.beginPath();
    context.rect(p.x, p.y, p.w, p.h);
    context.clip();
    context.setTransform(p.flipX ? -1 : 1, 0, 0, 1, p.x + (p.flipX ? p.w : 0), p.y);
    context.drawImage(source, p.sx, p.sy, p.sw, p.sh, 0, 0, p.w, p.h);
    context.restore();
  });
}
