#pragma once

#include "ofMain.h"
#include "view/viewConfiguration.h"

#include <string>
#include <vector>

class studySession {
public:
    struct Continuation {
        std::string participantId;
        std::string previousSessionId;
        viewConfiguration configuration;
    };

    static bool loadContinuation(
        const std::string& filePath,
        Continuation& continuation,
        std::string& error);

    bool start(
        const std::string& participantId,
        const std::string& administratorId,
        const viewConfiguration& configuration,
        const std::string& previousSessionId = "");
    bool recordPain(int painScore, const viewConfiguration& configuration);
    bool finishAndSave(const viewConfiguration& configuration);

    bool isActive() const;
    std::size_t painScoreCount() const;
    const std::string& lastError() const;
    const std::string& lastSavedPath() const;

private:
    struct PainMeasurement {
        double elapsedSeconds = 0.0;
        int painScore = 0;
        std::string recordedAt;
        viewConfiguration configuration;
    };

    static std::string currentTimestamp();
    static std::string newSessionId();
    ofJson makeJson(const std::string& endedAt) const;

    bool active = false;
    float startedAtElapsedSeconds = 0.0f;
    std::string sessionId;
    std::string participantId;
    std::string administratorId;
    std::string previousSessionId;
    std::string startedAt;
    viewConfiguration initialConfiguration;
    viewConfiguration finalConfiguration;
    std::vector<PainMeasurement> measurements;
    std::string error;
    std::string savedPath;
};
