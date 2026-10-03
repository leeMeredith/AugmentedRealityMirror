#pragma once

#include "ofPixels.h"

#include <memory>
#include <string>

class screenRecorder {
public:
    screenRecorder();
    ~screenRecorder();

    screenRecorder(const screenRecorder&) = delete;
    screenRecorder& operator=(const screenRecorder&) = delete;

    bool start(
        const std::string& outputPath,
        int width,
        int height,
        float frameRate);
    bool addFrame(const ofPixels& pixels, double elapsedSeconds);
    bool stop();

    bool isRecording() const;
    const std::string& lastError() const;
    const std::string& outputPath() const;

private:
    struct implementation;
    std::unique_ptr<implementation> impl;
};
