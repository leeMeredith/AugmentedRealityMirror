#pragma once

#include "ofxGui.h"
#include "view/mirrorRegions.h"

class mirrorControls {
public:
    mirrorControls() = default;
    ~mirrorControls();

    void setup();
    void draw();
    void toggle(mirrorRegions::Region region);
    void clear();

    mirrorRegions::Selection selection();

private:
    void clearPressed();

    ofxPanel panel;
    ofxToggle topLeft;
    ofxToggle topRight;
    ofxToggle bottomLeft;
    ofxToggle bottomRight;
    ofxButton clearButton;
};
