#pragma once

#include "ofMain.h"

#include <cctype>
#include <string>

namespace participantFileName {
inline std::string safeComponent(const std::string& participantId) {
    const std::string trimmed = ofTrim(participantId);
    std::string result;
    bool lastWasSeparator = false;

    for (const unsigned char character : trimmed) {
        const bool isSafeAscii = std::isalnum(character)
            || character == '-'
            || character == '_';
        const bool isUtf8Byte = character >= 128;

        if (isSafeAscii || isUtf8Byte) {
            result.push_back(static_cast<char>(character));
            lastWasSeparator = false;
        } else if (!result.empty() && !lastWasSeparator) {
            result.push_back('_');
            lastWasSeparator = true;
        }
    }

    while (!result.empty() && result.back() == '_') {
        result.pop_back();
    }
    return result.empty() ? "participant" : result;
}

inline std::string makeBaseName(
    const std::string& participantId,
    const std::string& datedId) {
    return safeComponent(participantId) + "_" + datedId;
}
}
