#pragma once

#include <array>

enum class rgbViewMode {
    regionalMirror,
    splitScreen
};

enum class splitOrientation {
    vertical,
    horizontal
};

struct viewConfiguration {
    using RegionSelection = std::array<bool, 4>;

    rgbViewMode mode = rgbViewMode::regionalMirror;
    RegionSelection oppositeCopyRegions{};
    splitOrientation splitDirection = splitOrientation::vertical;
    bool mirrorOnFirstSide = false;
    float verticalDividerPosition = 0.5f;
    float horizontalDividerPosition = 0.5f;
};
