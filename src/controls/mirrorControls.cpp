#include "mirrorControls.h"

mirrorControls::~mirrorControls() {
    clearButton.removeListener(this, &mirrorControls::clearPressed);
    resetDividersButton.removeListener(this, &mirrorControls::resetDividersPressed);
}

void mirrorControls::setup() {
    clearButton.addListener(this, &mirrorControls::clearPressed);
    resetDividersButton.addListener(this, &mirrorControls::resetDividersPressed);

    panel.setup("View Controls", "settings.json");
    panel.setPosition(12.0f, 12.0f);
    panel.add(splitScreen.setup("V  Split screen", false));
    panel.add(horizontalDivider.setup("Horizontal divider", false));
    panel.add(mirrorOnFirstSide.setup("Mirror left / top", false));
    panel.add(verticalDividerPosition.setup("Vertical position", 0.5f, 0.2f, 0.8f));
    panel.add(horizontalDividerPosition.setup("Horizontal position", 0.5f, 0.2f, 0.8f));
    panel.add(resetDividersButton.setup("Center both dividers"));
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
    return result;
}

void mirrorControls::clearPressed() {
    clear();
}

void mirrorControls::resetDividersPressed() {
    verticalDividerPosition = 0.5f;
    horizontalDividerPosition = 0.5f;
}
