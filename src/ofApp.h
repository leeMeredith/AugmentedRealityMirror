#pragma once

#include "ofMain.h"
#include "captureRGB.h"
#include "controls/mirrorControls.h"
#include "controls/sessionControls.h"
#include "recording/recordingControls.h"
#include "view/mirrorRegions.h"
#include "view/splitScreenView.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    void keyPressed(int key) override;

private:
    captureRGB rgbCamera;
    ofFbo viewFrame;
    mirrorRegions regionView;
    splitScreenView comparisonView;
    mirrorControls controls;
    sessionControls studyControls;
    recordingControls recording;
    bool showInterface = true;
};
