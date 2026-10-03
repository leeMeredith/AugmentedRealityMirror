#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);
    controls.setup();
    studyControls.setup();

    if (!rgbCamera.setup(0, 640, 480, 30)) {
        ofLogError("ofApp") << "Unable to initialize RGB camera device 0";
    }
}

void ofApp::update() {
    rgbCamera.update();
    studyControls.update(controls.configuration());
}

void ofApp::draw() {
    if (!rgbCamera.isReady()) {
        ofSetColor(255);
        ofDrawBitmapString("Waiting for RGB camera...", 20, 30);
        if (showInterface) {
            controls.draw();
            studyControls.draw();
        }
        return;
    }

    ofRectangle preview(0, 0, rgbCamera.getWidth(), rgbCamera.getHeight());
    preview.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);

    const auto configuration = controls.configuration();
    if (configuration.mode == rgbViewMode::splitScreen) {
        comparisonView.draw(
            rgbCamera.getTexture(),
            preview,
            configuration,
            showInterface);
    } else {
        regionView.draw(
            rgbCamera.getTexture(),
            preview,
            configuration.oppositeCopyRegions,
            showInterface);
    }

    if (showInterface) {
        controls.draw();
        studyControls.draw();
    }
}

void ofApp::exit() {
    studyControls.exit();
    rgbCamera.close();
}

void ofApp::keyPressed(int key) {
    if (key >= '1' && key <= '9') {
        studyControls.recordPainScore(key - '0', controls.configuration());
        return;
    }

    if (key == '0') {
        studyControls.recordPainScore(10, controls.configuration());
        return;
    }

    switch (key) {
        case 'q':
        case 'Q':
            controls.toggle(mirrorRegions::Region::topLeft);
            break;
        case 'w':
        case 'W':
            controls.toggle(mirrorRegions::Region::topRight);
            break;
        case 'a':
        case 'A':
            controls.toggle(mirrorRegions::Region::bottomLeft);
            break;
        case 's':
        case 'S':
            controls.toggle(mirrorRegions::Region::bottomRight);
            break;
        case 'v':
        case 'V':
            controls.toggleSplitScreen();
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
