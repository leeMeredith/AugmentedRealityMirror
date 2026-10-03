#pragma once

#include "ofMain.h"
#include "viewConfiguration.h"

class splitScreenView {
public:
    void draw(
        const ofTexture& texture,
        const ofRectangle& bounds,
        const viewConfiguration& configuration,
        bool showOverlay) const;
};
