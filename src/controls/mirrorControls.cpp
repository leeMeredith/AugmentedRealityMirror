#include "mirrorControls.h"

mirrorControls::~mirrorControls() {
    clearButton.removeListener(this, &mirrorControls::clearPressed);
}

void mirrorControls::setup() {
    clearButton.addListener(this, &mirrorControls::clearPressed);

    panel.setup("Mirror Regions");
    panel.setPosition(12.0f, 12.0f);
    panel.add(topLeft.setup("1  Top left", false));
    panel.add(topRight.setup("2  Top right", false));
    panel.add(bottomLeft.setup("3  Bottom left", false));
    panel.add(bottomRight.setup("4  Bottom right", false));
    panel.add(clearButton.setup("Reset all"));
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

void mirrorControls::clear() {
    topLeft = false;
    topRight = false;
    bottomLeft = false;
    bottomRight = false;
}

mirrorRegions::Selection mirrorControls::selection() {
    return {topLeft, topRight, bottomLeft, bottomRight};
}

void mirrorControls::clearPressed() {
    clear();
}
