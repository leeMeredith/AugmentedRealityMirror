#pragma once

#include "ofMain.h"

class captureRGB {
public:
    bool setup(int deviceId, int width, int height, int frameRate);
    void update();
    void close();

    bool isReady() const;
    const ofTexture& getTexture() const;
    float getWidth() const;
    float getHeight() const;

private:
    ofVideoGrabber camera;
};
