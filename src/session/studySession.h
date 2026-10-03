#pragma once

#include "ofMain.h"

#include <array>
#include <string>
#include <vector>

class studySession {
public:
    using RegionSelection = std::array<bool, 4>;

    bool start(const std::string& participantId, const std::string& administratorId);
    bool recordPain(int painScore, const RegionSelection& regions);
    bool finishAndSave();

    bool isActive() const;
    std::size_t painScoreCount() const;
    const std::string& lastError() const;
    const std::string& lastSavedPath() const;

private:
    struct PainMeasurement {
        double elapsedSeconds = 0.0;
        int painScore = 0;
        std::string recordedAt;
        RegionSelection regions{};
    };

    static std::string currentTimestamp();
    static std::string newSessionId();
    ofJson makeJson(const std::string& endedAt) const;

    bool active = false;
    float startedAtElapsedSeconds = 0.0f;
    std::string sessionId;
    std::string participantId;
    std::string administratorId;
    std::string startedAt;
    std::vector<PainMeasurement> measurements;
    std::string error;
    std::string savedPath;
};
