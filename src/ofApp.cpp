#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);

    if (!rgbCamera.setup(0, 640, 480, 30)) {
        ofLogError("ofApp") << "Unable to initialize RGB camera device 0";
    }
}

void ofApp::update() {
    rgbCamera.update();
}

void ofApp::draw() {
    if (!rgbCamera.isReady()) {
        ofSetColor(255);
        ofDrawBitmapString("Waiting for RGB camera...", 20, 30);
        return;
    }

    ofRectangle preview(0, 0, rgbCamera.getWidth(), rgbCamera.getHeight());
    preview.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);

    ofSetColor(255);
    rgbCamera.getTexture().draw(preview.x, preview.y, preview.width, preview.height);
}

void ofApp::exit() {
    rgbCamera.close();
}
