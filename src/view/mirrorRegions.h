#pragma once

#include "ofMain.h"

#include <array>
#include <cstddef>

class mirrorRegions {
public:
    enum class Region : std::size_t {
        topLeft = 0,
        topRight,
        bottomLeft,
        bottomRight,
        count
    };

    using Selection = std::array<bool, static_cast<std::size_t>(Region::count)>;

    void draw(
        const ofTexture& texture,
        const ofRectangle& bounds,
        const Selection& mirrored,
        bool showOverlay) const;
};
