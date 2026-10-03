#include "sessionControls.h"

sessionControls::~sessionControls() {
    startButton.removeListener(this, &sessionControls::startPressed);
    recordButton.removeListener(this, &sessionControls::recordPressed);
    finishButton.removeListener(this, &sessionControls::finishPressed);
}

void sessionControls::setup() {
    participantId.set("Participant ID", "");
    administratorId.set("Administrator ID", "");
    painScore.set("Pain score", 0, 0, 10);

    startButton.addListener(this, &sessionControls::startPressed);
    recordButton.addListener(this, &sessionControls::recordPressed);
    finishButton.addListener(this, &sessionControls::finishPressed);

    panel.setup("Study Session", "settings.json");
    panel.setPosition(230.0f, 12.0f);
    panel.add(participantId);
    panel.add(administratorId);
    panel.add(painScore);
    panel.add(painKeysLabel.setup("Pain keys", "1-9, 0 = 10"));
    panel.add(startButton.setup("Start session"));
    panel.add(recordButton.setup("Record pain score"));
    panel.add(finishButton.setup("Finish and save"));
    panel.add(statusLabel.setup("Status", "Ready"));
}

void sessionControls::update(const viewConfiguration& configuration) {
    if (startRequested) {
        startRequested = false;
        if (session.start(participantId, administratorId)) {
            statusLabel = "Session started";
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
        if (session.finishAndSave()) {
            statusLabel = "Saved " + ofFilePath::getFileName(session.lastSavedPath());
        } else {
            statusLabel = session.lastError();
        }
    }
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

void sessionControls::exit() {
    if (session.isActive() && !session.finishAndSave()) {
        ofLogError("sessionControls") << session.lastError();
    }
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
