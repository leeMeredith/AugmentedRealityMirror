#include "captureRGB.h"

bool captureRGB::setup(int deviceId, int width, int height, int frameRate) {
    const auto devices = camera.listDevices();
    for (const auto& device : devices) {
        ofLogNotice("captureRGB")
            << device.id << ": " << device.deviceName
            << (device.bAvailable ? "" : " - unavailable");
    }

    camera.setDeviceID(deviceId);
    camera.setDesiredFrameRate(frameRate);
    return camera.setup(width, height);
}

void captureRGB::update() {
    camera.update();
}

void captureRGB::close() {
    camera.close();
}

bool captureRGB::isReady() const {
    return camera.isInitialized();
}

const ofTexture& captureRGB::getTexture() const {
    return camera.getTexture();
}

float captureRGB::getWidth() const {
    return camera.getWidth();
}

float captureRGB::getHeight() const {
    return camera.getHeight();
}
