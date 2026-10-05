#include "sessionControls.h"

sessionControls::~sessionControls() {
    loadButton.removeListener(this, &sessionControls::loadPressed);
    startButton.removeListener(this, &sessionControls::startPressed);
    recordButton.removeListener(this, &sessionControls::recordPressed);
    finishButton.removeListener(this, &sessionControls::finishPressed);
}

void sessionControls::setup() {
    participantId.set("Participant ID", "");
    administratorId.set("Administrator ID", "");
    painScore.set("Pain score", 0, 0, 10);

    loadButton.addListener(this, &sessionControls::loadPressed);
    startButton.addListener(this, &sessionControls::startPressed);
    recordButton.addListener(this, &sessionControls::recordPressed);
    finishButton.addListener(this, &sessionControls::finishPressed);

    panel.setup("Study Session", "settings.json");
    panel.setPosition(230.0f, 12.0f);
    panel.add(participantId);
    panel.add(administratorId);
    panel.add(painScore);
    panel.add(painKeysLabel.setup("Pain keys", "1-9, 0 = 10"));
    panel.add(loadButton.setup("Load prior session"));
    panel.add(startButton.setup("Start session"));
    panel.add(recordButton.setup("Record pain score"));
    panel.add(finishButton.setup("Finish and save"));
    panel.add(statusLabel.setup("Status", "Ready"));
}

void sessionControls::update(const viewConfiguration& configuration) {
    if (loadRequested) {
        loadRequested = false;
        if (session.isActive()) {
            statusLabel = "Finish the active session first";
        } else {
            auto dialog = ofSystemLoadDialog(
                "Select a prior ARM session JSON file",
                false,
                ofToDataPath("sessions", true));
            if (!dialog.bSuccess) {
                statusLabel = "Load cancelled";
            } else {
                studySession::Continuation continuation;
                std::string loadError;
                if (studySession::loadContinuation(
                    dialog.getPath(),
                    continuation,
                    loadError)) {
                    participantId = continuation.participantId;
                    continuationParticipantId = continuation.participantId;
                    previousSessionId = continuation.previousSessionId;
                    loadedConfiguration = continuation.configuration;
                    statusLabel = "Prior setup loaded";
                } else {
                    statusLabel = loadError;
                }
            }
        }
    }

    if (startRequested) {
        startRequested = false;
        const bool continuePrevious = ofTrim(participantId.get())
            == continuationParticipantId;
        const std::string continuedFrom = continuePrevious
            ? previousSessionId
            : "";
        if (session.start(
            participantId,
            administratorId,
            configuration,
            continuedFrom)) {
            statusLabel = continuedFrom.empty()
                ? "Session started"
                : "Continued session started";
            continuationParticipantId.clear();
            previousSessionId.clear();
        } else {
            statusLabel = session.lastError();
        }
    }

    if (recordRequested) {
        recordRequested = false;
        recordPainScore(painScore.get(), configuration);
    }

    if (finishRequested) {
        finishRequested = false;
        if (session.finishAndSave(configuration)) {
            statusLabel = "Saved " + ofFilePath::getFileName(session.lastSavedPath());
        } else {
            statusLabel = session.lastError();
        }
    }
}

std::optional<viewConfiguration> sessionControls::takeLoadedConfiguration() {
    if (!loadedConfiguration.has_value()) {
        return std::nullopt;
    }

    const auto configuration = loadedConfiguration;
    loadedConfiguration.reset();
    return configuration;
}

std::string sessionControls::participantIdentifier() const {
    return ofTrim(participantId.get());
}

void sessionControls::recordPainScore(
    int selectedPainScore,
    const viewConfiguration& configuration) {
    const int score = ofClamp(selectedPainScore, 0, 10);
    painScore = score;

    if (session.recordPain(score, configuration)) {
        statusLabel = "Recorded " + ofToString(score)
            + " (#" + ofToString(session.painScoreCount()) + ")";
    } else {
        statusLabel = session.lastError();
    }
}

void sessionControls::draw() {
    panel.draw();
}

void sessionControls::exit(const viewConfiguration& configuration) {
    if (session.isActive() && !session.finishAndSave(configuration)) {
        ofLogError("sessionControls") << session.lastError();
    }
}

void sessionControls::loadPressed() {
    loadRequested = true;
}

void sessionControls::startPressed() {
    startRequested = true;
}

void sessionControls::recordPressed() {
    recordRequested = true;
}

void sessionControls::finishPressed() {
    finishRequested = true;
}
