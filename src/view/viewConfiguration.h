#pragma once

#include <array>

enum class rgbViewMode {
    regionalMirror,
    splitScreen
};

struct viewConfiguration {
    using RegionSelection = std::array<bool, 4>;

    rgbViewMode mode = rgbViewMode::regionalMirror;
    RegionSelection oppositeCopyRegions{};
    bool mirrorOnLeft = false;
    float dividerPosition = 0.5f;
};
