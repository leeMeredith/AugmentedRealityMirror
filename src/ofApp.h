#pragma once

#include "ofMain.h"
#include "captureRGB.h"
#include "controls/mirrorControls.h"
#include "controls/sessionControls.h"
#include "view/mirrorRegions.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    void keyPressed(int key) override;

private:
    captureRGB rgbCamera;
    mirrorRegions regionView;
    mirrorControls controls;
    sessionControls studyControls;
    bool showInterface = true;
};
