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

    ofxPanel panel;
    ofxToggle splitScreen;
    ofxToggle mirrorOnLeft;
    ofxFloatSlider dividerPosition;
    ofxToggle topLeft;
    ofxToggle topRight;
    ofxToggle bottomLeft;
    ofxToggle bottomRight;
    ofxButton clearButton;
};
