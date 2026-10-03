#include "studySession.h"

bool studySession::start(
    const std::string& requestedParticipantId,
    const std::string& requestedAdministratorId) {
    error.clear();
    savedPath.clear();

    if (active) {
        error = "Finish the active session first";
        return false;
    }

    const auto cleanParticipantId = ofTrim(requestedParticipantId);
    const auto cleanAdministratorId = ofTrim(requestedAdministratorId);
    if (cleanParticipantId.empty() || cleanAdministratorId.empty()) {
        error = "Enter both IDs";
        return false;
    }

    active = true;
    startedAtElapsedSeconds = ofGetElapsedTimef();
    sessionId = newSessionId();
    participantId = cleanParticipantId;
    administratorId = cleanAdministratorId;
    startedAt = currentTimestamp();
    measurements.clear();
    return true;
}

bool studySession::recordPain(int requestedPainScore, const RegionSelection& regions) {
    error.clear();
    if (!active) {
        error = "Start a session first";
        return false;
    }

    PainMeasurement measurement;
    measurement.elapsedSeconds = ofGetElapsedTimef() - startedAtElapsedSeconds;
    measurement.painScore = ofClamp(requestedPainScore, 0, 10);
    measurement.recordedAt = currentTimestamp();
    measurement.regions = regions;
    measurements.push_back(measurement);
    return true;
}

bool studySession::finishAndSave() {
    error.clear();
    savedPath.clear();
    if (!active) {
        error = "No active session";
        return false;
    }

    const std::string directoryPath = ofToDataPath("sessions", true);
    if (!ofDirectory::doesDirectoryExist(directoryPath, false)
        && !ofDirectory::createDirectory(directoryPath, false, true)) {
        error = "Could not create sessions folder";
        return false;
    }

    const std::string outputPath = ofFilePath::join(directoryPath, sessionId + ".json");
    if (!ofSavePrettyJson(outputPath, makeJson(currentTimestamp()))) {
        error = "Could not save session JSON";
        return false;
    }

    savedPath = outputPath;
    active = false;
    return true;
}

bool studySession::isActive() const {
    return active;
}

std::size_t studySession::painScoreCount() const {
    return measurements.size();
}

const std::string& studySession::lastError() const {
    return error;
}

const std::string& studySession::lastSavedPath() const {
    return savedPath;
}

std::string studySession::currentTimestamp() {
    return ofGetTimestampString("%Y-%m-%dT%H:%M:%S%z");
}

std::string studySession::newSessionId() {
    return ofGetTimestampString("%Y%m%d_%H%M%S")
        + "_"
        + ofToString(ofGetSystemTimeMillis() % 1000);
}

ofJson studySession::makeJson(const std::string& endedAt) const {
    ofJson json;
    json["schemaVersion"] = 1;
    json["application"]["name"] = "AugmentedRealityMirrorRGB";
    json["application"]["cameraMode"] = "RGB";

    json["session"]["id"] = sessionId;
    json["session"]["participantId"] = participantId;
    json["session"]["administratorId"] = administratorId;
    json["session"]["startedAt"] = startedAt;
    json["session"]["endedAt"] = endedAt;

    json["measurements"] = ofJson::array();
    ofJson chartSource = ofJson::array();

    for (std::size_t index = 0; index < measurements.size(); ++index) {
        const auto& measurement = measurements[index];
        ofJson entry;
        entry["index"] = index;
        entry["recordedAt"] = measurement.recordedAt;
        entry["elapsedSeconds"] = measurement.elapsedSeconds;
        entry["painScore"] = measurement.painScore;
        entry["mirrorRegions"]["topLeft"] = measurement.regions[0];
        entry["mirrorRegions"]["topRight"] = measurement.regions[1];
        entry["mirrorRegions"]["bottomLeft"] = measurement.regions[2];
        entry["mirrorRegions"]["bottomRight"] = measurement.regions[3];
        json["measurements"].push_back(entry);

        chartSource.push_back({measurement.elapsedSeconds, measurement.painScore});
    }

    json["echarts"]["dataset"]["dimensions"] = {"elapsedSeconds", "painScore"};
    json["echarts"]["dataset"]["source"] = chartSource;
    json["echarts"]["xAxis"] = {
        {"type", "value"},
        {"name", "Elapsed seconds"}
    };
    json["echarts"]["yAxis"] = {
        {"type", "value"},
        {"name", "Pain score"},
        {"min", 0},
        {"max", 10}
    };
    json["echarts"]["series"] = ofJson::array({{
        {"type", "line"},
        {"name", "Pain score"},
        {"encode", {{"x", "elapsedSeconds"}, {"y", "painScore"}}}
    }});

    return json;
}
