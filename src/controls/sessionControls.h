#pragma once

#include "ofxGui.h"
#include "session/studySession.h"
#include "view/viewConfiguration.h"

class sessionControls {
public:
    sessionControls() = default;
    ~sessionControls();

    void setup();
    void update(const viewConfiguration& configuration);
    void recordPainScore(int selectedPainScore, const viewConfiguration& configuration);
    void draw();
    void exit();

private:
    void startPressed();
    void recordPressed();
    void finishPressed();

    ofxPanel panel;
    ofParameter<std::string> participantId;
    ofParameter<std::string> administratorId;
    ofParameter<int> painScore;
    ofxLabel painKeysLabel;
    ofxButton startButton;
    ofxButton recordButton;
    ofxButton finishButton;
    ofxLabel statusLabel;

    studySession session;
    bool startRequested = false;
    bool recordRequested = false;
    bool finishRequested = false;
};
