#pragma once

#include "ofxGui.h"
#include "session/studySession.h"
#include "view/viewConfiguration.h"

#include <optional>

class sessionControls {
public:
    sessionControls() = default;
    ~sessionControls();

    void setup();
    void update(const viewConfiguration& configuration);
    void recordPainScore(int selectedPainScore, const viewConfiguration& configuration);
    std::optional<viewConfiguration> takeLoadedConfiguration();
    void draw();
    void exit(const viewConfiguration& configuration);

private:
    void loadPressed();
    void startPressed();
    void recordPressed();
    void finishPressed();

    ofxPanel panel;
    ofParameter<std::string> participantId;
    ofParameter<std::string> administratorId;
    ofParameter<int> painScore;
    ofxLabel painKeysLabel;
    ofxButton loadButton;
    ofxButton startButton;
    ofxButton recordButton;
    ofxButton finishButton;
    ofxLabel statusLabel;

    studySession session;
    std::optional<viewConfiguration> loadedConfiguration;
    std::string continuationParticipantId;
    std::string previousSessionId;
    bool loadRequested = false;
    bool startRequested = false;
    bool recordRequested = false;
    bool finishRequested = false;
};
