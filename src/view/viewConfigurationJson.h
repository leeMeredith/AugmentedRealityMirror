#pragma once

#include "ofMain.h"
#include "viewConfiguration.h"

#include <cmath>
#include <exception>
#include <string>

namespace viewConfigurationJson {
inline ofJson make(const viewConfiguration& configuration) {
    ofJson json;
    json["mode"] = configuration.mode == rgbViewMode::splitScreen
        ? "splitScreen"
        : "regionalMirror";

    const bool horizontalSplit = configuration.splitDirection
        == splitOrientation::horizontal;
    json["splitOrientation"] = horizontalSplit ? "horizontal" : "vertical";
    json["mirrorOnFirstSide"] = configuration.mirrorOnFirstSide;
    json["mirrorSide"] = horizontalSplit
        ? (configuration.mirrorOnFirstSide ? "top" : "bottom")
        : (configuration.mirrorOnFirstSide ? "left" : "right");
    json["activeDividerPosition"] = horizontalSplit
        ? configuration.horizontalDividerPosition
        : configuration.verticalDividerPosition;
    json["verticalDividerPosition"] = configuration.verticalDividerPosition;
    json["horizontalDividerPosition"] = configuration.horizontalDividerPosition;
    json["regionVerticalDividerPosition"] =
        configuration.regionVerticalDividerPosition;
    json["regionHorizontalDividerPosition"] =
        configuration.regionHorizontalDividerPosition;
    json["oppositeCopyRegions"]["topLeft"] = configuration.oppositeCopyRegions[0];
    json["oppositeCopyRegions"]["topRight"] = configuration.oppositeCopyRegions[1];
    json["oppositeCopyRegions"]["bottomLeft"] = configuration.oppositeCopyRegions[2];
    json["oppositeCopyRegions"]["bottomRight"] = configuration.oppositeCopyRegions[3];
    return json;
}

inline bool read(
    const ofJson& json,
    viewConfiguration& configuration,
    std::string& error) {
    error.clear();
    if (!json.is_object()) {
        error = "Saved view settings are not an object";
        return false;
    }

    try {
        configuration.mode = json.value("mode", "regionalMirror") == "splitScreen"
            ? rgbViewMode::splitScreen
            : rgbViewMode::regionalMirror;
        configuration.splitDirection =
            json.value("splitOrientation", "vertical") == "horizontal"
            ? splitOrientation::horizontal
            : splitOrientation::vertical;
        configuration.mirrorOnFirstSide = json.value(
            "mirrorOnFirstSide",
            json.value("mirrorOnLeft", false));
        configuration.verticalDividerPosition = json.value(
            "verticalDividerPosition",
            json.value("dividerPosition", 0.5f));
        configuration.horizontalDividerPosition = json.value(
            "horizontalDividerPosition",
            0.5f);
        configuration.regionVerticalDividerPosition = json.value(
            "regionVerticalDividerPosition",
            0.5f);
        configuration.regionHorizontalDividerPosition = json.value(
            "regionHorizontalDividerPosition",
            0.5f);

        const auto regions = json.find("oppositeCopyRegions");
        if (regions != json.end() && regions->is_object()) {
            configuration.oppositeCopyRegions[0] = regions->value("topLeft", false);
            configuration.oppositeCopyRegions[1] = regions->value("topRight", false);
            configuration.oppositeCopyRegions[2] = regions->value("bottomLeft", false);
            configuration.oppositeCopyRegions[3] = regions->value("bottomRight", false);
        }
    } catch (const std::exception& exception) {
        error = "Could not read saved view settings: ";
        error += exception.what();
        return false;
    }

    return true;
}

inline bool matches(
    const viewConfiguration& first,
    const viewConfiguration& second) {
    return first.mode == second.mode
        && first.oppositeCopyRegions == second.oppositeCopyRegions
        && first.splitDirection == second.splitDirection
        && first.mirrorOnFirstSide == second.mirrorOnFirstSide
        && std::abs(
            first.verticalDividerPosition - second.verticalDividerPosition) < 0.001f
        && std::abs(
            first.horizontalDividerPosition - second.horizontalDividerPosition) < 0.001f
        && std::abs(
            first.regionVerticalDividerPosition
                - second.regionVerticalDividerPosition) < 0.001f
        && std::abs(
            first.regionHorizontalDividerPosition
                - second.regionHorizontalDividerPosition) < 0.001f;
}
}
