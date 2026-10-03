#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);
    controls.setup();
    studyControls.setup();
    recording.setup();

    if (!rgbCamera.setup(0, 640, 480, 30)) {
        ofLogError("ofApp") << "Unable to initialize RGB camera device 0";
    }
}

void ofApp::update() {
    rgbCamera.update();

    const auto configuration = controls.configuration();
    studyControls.update(configuration);
    recording.update(
        rgbCamera.isReady() ? static_cast<int>(rgbCamera.getWidth()) : 0,
        rgbCamera.isReady() ? static_cast<int>(rgbCamera.getHeight()) : 0,
        configuration);
}

void ofApp::draw() {
    if (!rgbCamera.isReady()) {
        ofSetColor(255);
        ofDrawBitmapString("Waiting for RGB camera...", 20, 30);
        if (showInterface) {
            controls.draw();
            studyControls.draw();
            recording.draw();
        }
        return;
    }

    const int frameWidth = static_cast<int>(rgbCamera.getWidth());
    const int frameHeight = static_cast<int>(rgbCamera.getHeight());
    if (!viewFrame.isAllocated()
        || viewFrame.getWidth() != frameWidth
        || viewFrame.getHeight() != frameHeight) {
        viewFrame.allocate(frameWidth, frameHeight, GL_RGBA);
    }

    const auto configuration = controls.configuration();
    const ofRectangle frameBounds(0, 0, frameWidth, frameHeight);

    viewFrame.begin();
    ofClear(0, 0, 0, 255);
    if (configuration.mode == rgbViewMode::splitScreen) {
        comparisonView.draw(
            rgbCamera.getTexture(),
            frameBounds,
            configuration,
            showInterface);
    } else {
        regionView.draw(
            rgbCamera.getTexture(),
            frameBounds,
            configuration.oppositeCopyRegions,
            configuration.regionVerticalDividerPosition,
            configuration.regionHorizontalDividerPosition,
            showInterface);
    }
    viewFrame.end();

    ofRectangle preview(0, 0, frameWidth, frameHeight);
    preview.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
    ofSetColor(255);
    viewFrame.draw(preview);

    if (rgbCamera.isFrameNew()) {
        recording.captureFrame(viewFrame, configuration);
    }

    if (showInterface) {
        controls.draw();
        studyControls.draw();
        recording.draw();
    }
}

void ofApp::exit() {
    recording.exit();
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
