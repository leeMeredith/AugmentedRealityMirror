#pragma once

#include "ofMain.h"
#include "viewConfiguration.h"

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

    using Selection = viewConfiguration::RegionSelection;

    void draw(
        const ofTexture& texture,
        const ofRectangle& bounds,
        const Selection& mirrored,
        float verticalDividerPosition,
        float horizontalDividerPosition,
        bool showOverlay) const;
};
