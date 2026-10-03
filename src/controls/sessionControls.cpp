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

    panel.setup("Study Session");
    panel.setPosition(230.0f, 12.0f);
    panel.add(participantId);
    panel.add(administratorId);
    panel.add(painScore);
    panel.add(startButton.setup("Start session"));
    panel.add(recordButton.setup("Record pain score"));
    panel.add(finishButton.setup("Finish and save"));
    panel.add(statusLabel.setup("Status", "Ready"));
}

void sessionControls::update(const mirrorRegions::Selection& regions) {
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
        if (session.recordPain(painScore, regions)) {
            statusLabel = "Recorded #" + ofToString(session.painScoreCount());
        } else {
            statusLabel = session.lastError();
        }
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
