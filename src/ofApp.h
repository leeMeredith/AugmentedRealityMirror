#pragma once

#include "ofMain.h"
#include "captureRGB.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

private:
    captureRGB rgbCamera;
};
