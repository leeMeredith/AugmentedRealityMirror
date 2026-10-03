#include "studySession.h"
#include "view/viewConfigurationJson.h"

bool studySession::loadContinuation(
    const std::string& filePath,
    Continuation& continuation,
    std::string& loadError) {
    loadError.clear();

    try {
        const ofJson json = ofLoadJson(filePath);
        if (!json.is_object() || !json.contains("session")) {
            loadError = "The selected file is not a study session";
            return false;
        }

        const auto& savedSession = json.at("session");
        continuation.participantId = savedSession.value("participantId", "");
        continuation.previousSessionId = savedSession.value("id", "");
        if (continuation.participantId.empty()
            || continuation.previousSessionId.empty()) {
            loadError = "The selected session is missing its participant or session ID";
            return false;
        }

        const ofJson* savedView = nullptr;
        ofJson legacyView;
        if (savedSession.contains("viewAtEnd")
            && savedSession.at("viewAtEnd").is_object()) {
            savedView = &savedSession.at("viewAtEnd");
        } else if (json.contains("measurements")
            && json.at("measurements").is_array()) {
            const auto& measurements = json.at("measurements");
            for (auto entry = measurements.rbegin(); entry != measurements.rend(); ++entry) {
                if (entry->contains("view") && entry->at("view").is_object()) {
                    savedView = &entry->at("view");
                    break;
                }
                if (entry->contains("mirrorRegions")
                    && entry->at("mirrorRegions").is_object()) {
                    const auto& legacyRegions = entry->at("mirrorRegions");
                    legacyView["mode"] = "regionalMirror";
                    legacyView["oppositeCopyRegions"]["topLeft"] =
                        !legacyRegions.value("topLeft", false);
                    legacyView["oppositeCopyRegions"]["topRight"] =
                        !legacyRegions.value("topRight", false);
                    legacyView["oppositeCopyRegions"]["bottomLeft"] =
                        !legacyRegions.value("bottomLeft", false);
                    legacyView["oppositeCopyRegions"]["bottomRight"] =
                        !legacyRegions.value("bottomRight", false);
                    savedView = &legacyView;
                    break;
                }
            }
        }

        if (savedView == nullptr
            && savedSession.contains("viewAtStart")
            && savedSession.at("viewAtStart").is_object()) {
            savedView = &savedSession.at("viewAtStart");
        }
        if (savedView == nullptr) {
            loadError = "The selected session has no saved view settings";
            return false;
        }

        return viewConfigurationJson::read(
            *savedView,
            continuation.configuration,
            loadError);
    } catch (const std::exception& exception) {
        loadError = "Could not load the selected session: ";
        loadError += exception.what();
        return false;
    }
}

bool studySession::start(
    const std::string& requestedParticipantId,
    const std::string& requestedAdministratorId,
    const viewConfiguration& configuration,
    const std::string& requestedPreviousSessionId) {
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
    previousSessionId = requestedPreviousSessionId;
    startedAt = currentTimestamp();
    initialConfiguration = configuration;
    finalConfiguration = configuration;
    measurements.clear();
    return true;
}

bool studySession::recordPain(
    int requestedPainScore,
    const viewConfiguration& configuration) {
    error.clear();
    if (!active) {
        error = "Start a session first";
        return false;
    }

    PainMeasurement measurement;
    measurement.elapsedSeconds = ofGetElapsedTimef() - startedAtElapsedSeconds;
    measurement.painScore = ofClamp(requestedPainScore, 0, 10);
    measurement.recordedAt = currentTimestamp();
    measurement.configuration = configuration;
    measurements.push_back(measurement);
    return true;
}

bool studySession::finishAndSave(const viewConfiguration& configuration) {
    error.clear();
    savedPath.clear();
    if (!active) {
        error = "No active session";
        return false;
    }

    finalConfiguration = configuration;
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
    std::string timestamp = ofGetTimestampString("%Y-%m-%dT%H:%M:%S%z");
    if (timestamp.size() >= 5) {
        const std::size_t offsetStart = timestamp.size() - 5;
        if ((timestamp[offsetStart] == '+' || timestamp[offsetStart] == '-')
            && timestamp[timestamp.size() - 3] != ':') {
            timestamp.insert(timestamp.size() - 2, ":");
        }
    }
    return timestamp;
}

std::string studySession::newSessionId() {
    return ofGetTimestampString("%Y%m%d_%H%M%S")
        + "_"
        + ofToString(ofGetSystemTimeMillis() % 1000);
}

ofJson studySession::makeJson(const std::string& endedAt) const {
    ofJson json;
    json["schemaVersion"] = 5;
    json["application"]["name"] = "AugmentedRealityMirrorRGB";
    json["application"]["cameraMode"] = "RGB";

    json["session"]["id"] = sessionId;
    json["session"]["participantId"] = participantId;
    json["session"]["administratorId"] = administratorId;
    json["session"]["startedAt"] = startedAt;
    json["session"]["endedAt"] = endedAt;
    if (!previousSessionId.empty()) {
        json["session"]["continuedFromSessionId"] = previousSessionId;
    }
    json["session"]["viewAtStart"] =
        viewConfigurationJson::make(initialConfiguration);
    json["session"]["viewAtEnd"] =
        viewConfigurationJson::make(finalConfiguration);

    json["measurements"] = ofJson::array();
    ofJson chartSource = ofJson::array();
    ofJson longitudinalSource = ofJson::array();

    for (std::size_t index = 0; index < measurements.size(); ++index) {
        const auto& measurement = measurements[index];
        ofJson entry;
        entry["index"] = index;
        entry["recordedAt"] = measurement.recordedAt;
        entry["elapsedSeconds"] = measurement.elapsedSeconds;
        entry["painScore"] = measurement.painScore;
        entry["view"] = viewConfigurationJson::make(measurement.configuration);
        json["measurements"].push_back(entry);

        chartSource.push_back({measurement.elapsedSeconds, measurement.painScore});
        longitudinalSource.push_back({
            measurement.recordedAt,
            measurement.painScore,
            sessionId,
            measurement.elapsedSeconds
        });
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

    json["echarts"]["longitudinalDataset"]["dimensions"] = {
        "recordedAt",
        "painScore",
        "sessionId",
        "elapsedSeconds"
    };
    json["echarts"]["longitudinalDataset"]["source"] = longitudinalSource;

    return json;
}
