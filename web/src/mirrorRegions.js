// Quadrants are named in screen coordinates, after the overall camera orientation.
export const REGION_IDS = ["topLeft", "topRight", "bottomLeft", "bottomRight"];
export const REGION_LABELS = ["Top left", "Top right", "Bottom left", "Bottom right"];
export const defaultRegion = () => ({ mirrorX: false, mirrorY: false, offsetX: 0, offsetY: 0 });
export const defaultRegions = () => Object.fromEntries(REGION_IDS.map(id => [id, defaultRegion()]));

export function validateRegions(regions) {
  const result = {};
  for (const id of REGION_IDS) {
    const value = regions?.[id];
    if (!value || typeof value.mirrorX !== "boolean" || typeof value.mirrorY !== "boolean"
      || !Number.isFinite(value.offsetX) || Math.abs(value.offsetX) > 50
      || !Number.isFinite(value.offsetY) || Math.abs(value.offsetY) > 50) {
      throw new Error("The setup has invalid quadrant settings. Your current setup has not been changed.");
    }
    result[id] = { mirrorX: value.mirrorX, mirrorY: value.mirrorY, offsetX: value.offsetX, offsetY: value.offsetY };
  }
  return result;
}

export function hasRegionAdjustments(regions) {
  return REGION_IDS.some(id => Object.values(regions[id]).some(Boolean));
}

// Draw directly from the original frame, never from another processed quadrant.
// Offsets are percentages of quadrant size; clipping leaves uncovered areas dark.
export function drawRegions(context, source, width, height, mirrored, regions) {
  const w = width / 2, h = height / 2;
  context.setTransform(1, 0, 0, 1, 0, 0);
  context.fillStyle = "#142f2c";
  context.fillRect(0, 0, width, height);
  REGION_IDS.forEach((id, index) => {
    const region = regions[id];
    const column = index % 2, row = Math.floor(index / 2);
    const screenColumn = region.mirrorX ? 1 - column : column;
    const sourceColumn = mirrored ? 1 - screenColumn : screenColumn;
    const sourceRow = region.mirrorY ? 1 - row : row;
    const flipX = mirrored !== region.mirrorX;
    const x = column * w, y = row * h;
    context.save();
    context.beginPath();
    context.rect(x, y, w, h);
    context.clip();
    context.setTransform(flipX ? -1 : 1, 0, 0, region.mirrorY ? -1 : 1,
      x + region.offsetX * w / 100 + (flipX ? w : 0),
      y + region.offsetY * h / 100 + (region.mirrorY ? h : 0));
    context.drawImage(source, sourceColumn * w, sourceRow * h, w, h, 0, 0, w, h);
    context.restore();
  });
}
