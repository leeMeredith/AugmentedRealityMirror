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

    void draw(const ofTexture& texture, const ofRectangle& bounds) const;
    void toggle(Region region);
    void clear();

    bool isMirrored(Region region) const;

private:
    static std::size_t indexFor(Region region);

    std::array<bool, static_cast<std::size_t>(Region::count)> mirrored{};
};
