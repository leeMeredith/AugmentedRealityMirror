#include "recordingControls.h"
#include "session/participantFileName.h"
#include "view/viewConfigurationJson.h"

recordingControls::~recordingControls() {
    if (recorder.isRecording()) {
        stopRecording();
    }
    startButton.removeListener(this, &recordingControls::startPressed);
    stopButton.removeListener(this, &recordingControls::stopPressed);
}

void recordingControls::setup() {
    startButton.addListener(this, &recordingControls::startPressed);
    stopButton.addListener(this, &recordingControls::stopPressed);

    panel.setup("View Recording", "settings.json");
    panel.setPosition(448.0f, 12.0f);
    panel.add(formatLabel.setup("Format", "Silent MOV + JSON"));
    panel.add(startButton.setup("Start recording"));
    panel.add(stopButton.setup("Stop and save"));
    panel.add(statusLabel.setup("Status", "Ready"));
}

void recordingControls::update(
    const std::string& participantId,
    int frameWidth,
    int frameHeight,
    const viewConfiguration& configuration) {
    if (stopRequested) {
        stopRequested = false;
        stopRecording();
    }

    if (startRequested) {
        const std::string cleanParticipantId = ofTrim(participantId);
        if (cleanParticipantId.empty()) {
            startRequested = false;
            statusLabel = "Enter Participant ID first";
        } else if (frameWidth <= 0 || frameHeight <= 0) {
            statusLabel = "Waiting for camera";
        } else {
            startRequested = false;
            startRecording(
                cleanParticipantId,
                frameWidth,
                frameHeight,
                configuration);
        }
    }

    if (recorder.isRecording()) {
        appendConfiguration(configuration);
    }
}

void recordingControls::captureFrame(
    const ofFbo& frame,
    const viewConfiguration& configuration) {
    if (!recorder.isRecording() || !frame.isAllocated()) {
        return;
    }

    appendConfiguration(configuration);
    const double elapsedSeconds = ofGetElapsedTimef() - recordingStartedAtElapsed;
    if (elapsedSeconds + 0.0001 < nextFrameTime) {
        return;
    }

    frame.readToPixels(framePixels);
    if (!recorder.addFrame(framePixels, elapsedSeconds)) {
        statusLabel = recorder.lastError();
        stopRequested = true;
        return;
    }

    ++framesSubmitted;
    nextFrameTime += 1.0 / targetFrameRate;
    if (nextFrameTime < elapsedSeconds - 1.0) {
        nextFrameTime = elapsedSeconds + 1.0 / targetFrameRate;
    }
}

void recordingControls::draw() {
    panel.draw();
}

void recordingControls::exit() {
    if (recorder.isRecording()) {
        stopRecording();
    }
}

void recordingControls::startPressed() {
    stopRequested = false;
    startRequested = true;
}

void recordingControls::stopPressed() {
    startRequested = false;
    stopRequested = true;
}

bool recordingControls::startRecording(
    const std::string& participantId,
    int frameWidth,
    int frameHeight,
    const viewConfiguration& configuration) {
    if (recorder.isRecording()) {
        statusLabel = "Already recording";
        return false;
    }

    recordingParticipantId = ofTrim(participantId);
    if (recordingParticipantId.empty()) {
        statusLabel = "Enter Participant ID first";
        return false;
    }

    const std::string directoryPath = ofToDataPath("recordings", true);
    if (!ofDirectory::doesDirectoryExist(directoryPath, false)
        && !ofDirectory::createDirectory(directoryPath, false, true)) {
        statusLabel = "Could not create recordings folder";
        return false;
    }

    recordingId = participantFileName::makeBaseName(
        recordingParticipantId,
        newRecordingId());
    const std::string videoPath = ofFilePath::join(directoryPath, recordingId + ".mov");
    metadataPath = ofFilePath::join(directoryPath, recordingId + ".json");

    if (!recorder.start(videoPath, frameWidth, frameHeight, targetFrameRate)) {
        statusLabel = recorder.lastError();
        return false;
    }

    recordingWidth = frameWidth;
    recordingHeight = frameHeight;
    recordingStartedAtElapsed = ofGetElapsedTimef();
    nextFrameTime = 0.0;
    framesSubmitted = 0;
    startedAt = currentTimestamp();
    configurationTimeline.clear();
    hasLastConfiguration = false;
    appendConfiguration(configuration);
    statusLabel = "Recording";
    return true;
}

void recordingControls::stopRecording() {
    if (!recorder.isRecording()) {
        statusLabel = "No active recording";
        return;
    }

    const bool videoCompleted = recorder.stop();
    const std::string videoError = videoCompleted ? "" : recorder.lastError();
    const bool metadataSaved = saveMetadata(videoCompleted, videoError);

    if (!videoCompleted) {
        statusLabel = videoError;
    } else if (!metadataSaved) {
        statusLabel = "Video saved; metadata failed";
    } else {
        statusLabel = "Saved " + recordingId + ".mov";
    }
}

void recordingControls::appendConfiguration(const viewConfiguration& configuration) {
    if (hasLastConfiguration
        && viewConfigurationJson::matches(lastConfiguration, configuration)) {
        return;
    }

    ConfigurationEvent event;
    event.elapsedSeconds = recorder.isRecording()
        ? ofGetElapsedTimef() - recordingStartedAtElapsed
        : 0.0;
    event.configuration = configuration;
    configurationTimeline.push_back(event);
    lastConfiguration = configuration;
    hasLastConfiguration = true;
}

bool recordingControls::saveMetadata(
    bool videoCompleted,
    const std::string& videoError) {
    ofJson json;
    json["schemaVersion"] = 4;
    json["recordingId"] = recordingId;
    json["participantId"] = recordingParticipantId;
    json["videoFile"] = recordingId + ".mov";
    json["videoCompleted"] = videoCompleted;
    if (!videoError.empty()) {
        json["videoError"] = videoError;
    }
    json["startedAt"] = startedAt;
    json["endedAt"] = currentTimestamp();
    json["durationSeconds"] = ofGetElapsedTimef() - recordingStartedAtElapsed;
    json["width"] = recordingWidth;
    json["height"] = recordingHeight;
    json["targetFrameRate"] = targetFrameRate;
    json["framesSubmitted"] = framesSubmitted;
    json["configurationTimeline"] = ofJson::array();

    for (const auto& event : configurationTimeline) {
        ofJson entry = viewConfigurationJson::make(event.configuration);
        entry["elapsedSeconds"] = event.elapsedSeconds;
        json["configurationTimeline"].push_back(entry);
    }

    return ofSavePrettyJson(metadataPath, json);
}

std::string recordingControls::currentTimestamp() {
    return ofGetTimestampString("%Y-%m-%dT%H:%M:%S%z");
}

std::string recordingControls::newRecordingId() {
    return ofGetTimestampString("%Y%m%d_%H%M%S")
        + "_"
        + ofToString(ofGetSystemTimeMillis() % 1000);
}
