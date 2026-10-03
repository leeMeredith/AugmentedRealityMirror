#pragma once

#include "ofxGui.h"
#include "view/mirrorRegions.h"
#include "view/viewConfiguration.h"

class mirrorControls {
public:
    mirrorControls() = default;
    ~mirrorControls();

    void setup();
    void draw();
    void toggle(mirrorRegions::Region region);
    void toggleSplitScreen();
    void clear();
    void applyConfiguration(const viewConfiguration& configuration);

    viewConfiguration configuration();

private:
    void clearPressed();
    void resetSplitDividersPressed();
    void resetRegionDividersPressed();

    ofxPanel panel;
    ofxToggle splitScreen;
    ofxToggle horizontalDivider;
    ofxToggle mirrorOnFirstSide;
    ofxFloatSlider verticalDividerPosition;
    ofxFloatSlider horizontalDividerPosition;
    ofxButton resetSplitDividersButton;
    ofxFloatSlider regionVerticalDividerPosition;
    ofxFloatSlider regionHorizontalDividerPosition;
    ofxButton resetRegionDividersButton;
    ofxToggle topLeft;
    ofxToggle topRight;
    ofxToggle bottomLeft;
    ofxToggle bottomRight;
    ofxButton clearButton;
};
