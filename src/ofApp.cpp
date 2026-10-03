#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);
    controls.setup();

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
        if (showInterface) {
            controls.draw();
        }
        return;
    }

    ofRectangle preview(0, 0, rgbCamera.getWidth(), rgbCamera.getHeight());
    preview.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
    regionView.draw(
        rgbCamera.getTexture(),
        preview,
        controls.selection(),
        showInterface);

    if (showInterface) {
        controls.draw();
    }
}

void ofApp::exit() {
    rgbCamera.close();
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case '1':
            controls.toggle(mirrorRegions::Region::topLeft);
            break;
        case '2':
            controls.toggle(mirrorRegions::Region::topRight);
            break;
        case '3':
            controls.toggle(mirrorRegions::Region::bottomLeft);
            break;
        case '4':
            controls.toggle(mirrorRegions::Region::bottomRight);
            break;
        case 'r':
        case 'R':
            controls.clear();
            break;
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
        case 'h':
        case 'H':
            showInterface = !showInterface;
            break;
        default:
            break;
    }
}
