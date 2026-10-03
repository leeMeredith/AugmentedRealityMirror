#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "screenRecorder.h"
#include "view/viewConfiguration.h"

#include <string>
#include <vector>

class recordingControls {
public:
    recordingControls() = default;
    ~recordingControls();

    void setup();
    void update(int frameWidth, int frameHeight, const viewConfiguration& configuration);
    void captureFrame(const ofFbo& frame, const viewConfiguration& configuration);
    void draw();
    void exit();

private:
    struct ConfigurationEvent {
        double elapsedSeconds = 0.0;
        viewConfiguration configuration;
    };

    void startPressed();
    void stopPressed();
    bool startRecording(
        int frameWidth,
        int frameHeight,
        const viewConfiguration& configuration);
    void stopRecording();
    void appendConfiguration(const viewConfiguration& configuration);
    bool saveMetadata(bool videoCompleted, const std::string& videoError);

    static std::string currentTimestamp();
    static std::string newRecordingId();

    ofxPanel panel;
    ofxLabel formatLabel;
    ofxButton startButton;
    ofxButton stopButton;
    ofxLabel statusLabel;

    screenRecorder recorder;
    ofPixels framePixels;
    std::vector<ConfigurationEvent> configurationTimeline;
    viewConfiguration lastConfiguration;
    bool hasLastConfiguration = false;
    bool startRequested = false;
    bool stopRequested = false;
    double recordingStartedAtElapsed = 0.0;
    double nextFrameTime = 0.0;
    std::size_t framesSubmitted = 0;
    int recordingWidth = 0;
    int recordingHeight = 0;
    float targetFrameRate = 30.0f;
    std::string recordingId;
    std::string metadataPath;
    std::string startedAt;
};
