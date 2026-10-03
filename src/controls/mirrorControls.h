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

    viewConfiguration configuration();

private:
    void clearPressed();
    void resetDividersPressed();

    ofxPanel panel;
    ofxToggle splitScreen;
    ofxToggle horizontalDivider;
    ofxToggle mirrorOnFirstSide;
    ofxFloatSlider verticalDividerPosition;
    ofxFloatSlider horizontalDividerPosition;
    ofxButton resetDividersButton;
    ofxToggle topLeft;
    ofxToggle topRight;
    ofxToggle bottomLeft;
    ofxToggle bottomRight;
    ofxButton clearButton;
};
