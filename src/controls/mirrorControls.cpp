#include "mirrorControls.h"

mirrorControls::~mirrorControls() {
    clearButton.removeListener(this, &mirrorControls::clearPressed);
    resetSplitDividersButton.removeListener(
        this,
        &mirrorControls::resetSplitDividersPressed);
    resetRegionDividersButton.removeListener(
        this,
        &mirrorControls::resetRegionDividersPressed);
}

void mirrorControls::setup() {
    clearButton.addListener(this, &mirrorControls::clearPressed);
    resetSplitDividersButton.addListener(
        this,
        &mirrorControls::resetSplitDividersPressed);
    resetRegionDividersButton.addListener(
        this,
        &mirrorControls::resetRegionDividersPressed);

    panel.setup("View Controls", "settings.json");
    panel.setPosition(12.0f, 12.0f);
    panel.add(splitScreen.setup("V  Split screen", false));
    panel.add(horizontalDivider.setup("Horizontal divider", false));
    panel.add(mirrorOnFirstSide.setup("Mirror left / top", false));
    panel.add(verticalDividerPosition.setup("Split vertical", 0.5f, 0.2f, 0.8f));
    panel.add(horizontalDividerPosition.setup("Split horizontal", 0.5f, 0.2f, 0.8f));
    panel.add(resetSplitDividersButton.setup("Center split dividers"));
    panel.add(regionVerticalDividerPosition.setup("Quad vertical", 0.5f, 0.2f, 0.8f));
    panel.add(regionHorizontalDividerPosition.setup("Quad horizontal", 0.5f, 0.2f, 0.8f));
    panel.add(resetRegionDividersButton.setup("Center quad dividers"));
    panel.add(topLeft.setup("Q  Top left", false));
    panel.add(topRight.setup("W  Top right", false));
    panel.add(bottomLeft.setup("A  Bottom left", false));
    panel.add(bottomRight.setup("S  Bottom right", false));
    panel.add(clearButton.setup("Reset regions"));
}

void mirrorControls::draw() {
    panel.draw();
}

void mirrorControls::toggle(mirrorRegions::Region region) {
    switch (region) {
        case mirrorRegions::Region::topLeft:
            topLeft = !topLeft;
            break;
        case mirrorRegions::Region::topRight:
            topRight = !topRight;
            break;
        case mirrorRegions::Region::bottomLeft:
            bottomLeft = !bottomLeft;
            break;
        case mirrorRegions::Region::bottomRight:
            bottomRight = !bottomRight;
            break;
        case mirrorRegions::Region::count:
            break;
    }
}

void mirrorControls::toggleSplitScreen() {
    splitScreen = !splitScreen;
}

void mirrorControls::clear() {
    topLeft = false;
    topRight = false;
    bottomLeft = false;
    bottomRight = false;
}

void mirrorControls::applyConfiguration(const viewConfiguration& configuration) {
    splitScreen = configuration.mode == rgbViewMode::splitScreen;
    horizontalDivider = configuration.splitDirection == splitOrientation::horizontal;
    mirrorOnFirstSide = configuration.mirrorOnFirstSide;
    verticalDividerPosition = configuration.verticalDividerPosition;
    horizontalDividerPosition = configuration.horizontalDividerPosition;
    regionVerticalDividerPosition = configuration.regionVerticalDividerPosition;
    regionHorizontalDividerPosition = configuration.regionHorizontalDividerPosition;
    topLeft = configuration.oppositeCopyRegions[0];
    topRight = configuration.oppositeCopyRegions[1];
    bottomLeft = configuration.oppositeCopyRegions[2];
    bottomRight = configuration.oppositeCopyRegions[3];
}

viewConfiguration mirrorControls::configuration() {
    viewConfiguration result;
    result.mode = splitScreen ? rgbViewMode::splitScreen : rgbViewMode::regionalMirror;
    result.oppositeCopyRegions = {topLeft, topRight, bottomLeft, bottomRight};
    result.splitDirection = horizontalDivider
        ? splitOrientation::horizontal
        : splitOrientation::vertical;
    result.mirrorOnFirstSide = mirrorOnFirstSide;
    result.verticalDividerPosition = verticalDividerPosition;
    result.horizontalDividerPosition = horizontalDividerPosition;
    result.regionVerticalDividerPosition = regionVerticalDividerPosition;
    result.regionHorizontalDividerPosition = regionHorizontalDividerPosition;
    return result;
}

void mirrorControls::clearPressed() {
    clear();
}

void mirrorControls::resetSplitDividersPressed() {
    verticalDividerPosition = 0.5f;
    horizontalDividerPosition = 0.5f;
}

void mirrorControls::resetRegionDividersPressed() {
    regionVerticalDividerPosition = 0.5f;
    regionHorizontalDividerPosition = 0.5f;
}
