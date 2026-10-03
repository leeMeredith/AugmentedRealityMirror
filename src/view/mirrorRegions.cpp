#include "mirrorRegions.h"

namespace {
constexpr std::size_t regionCount = 4;

const char* regionLabel(std::size_t index) {
    static constexpr std::array<const char*, regionCount> labels{
        "1  TOP LEFT",
        "2  TOP RIGHT",
        "3  BOTTOM LEFT",
        "4  BOTTOM RIGHT"
    };
    return labels.at(index);
}
}

void mirrorRegions::draw(const ofTexture& texture, const ofRectangle& bounds) const {
    if (!texture.isAllocated() || bounds.isEmpty()) {
        return;
    }

    const float destinationWidth = bounds.getWidth() * 0.5f;
    const float destinationHeight = bounds.getHeight() * 0.5f;
    const float sourceWidth = texture.getWidth() * 0.5f;
    const float sourceHeight = texture.getHeight() * 0.5f;

    ofPushStyle();
    ofSetColor(255);

    for (std::size_t index = 0; index < regionCount; ++index) {
        const std::size_t column = index % 2;
        const std::size_t row = index / 2;
        const std::size_t sourceColumn = mirrored[index] ? 1 - column : column;

        const float destinationX = bounds.getX() + column * destinationWidth;
        const float destinationY = bounds.getY() + row * destinationHeight;
        const float sourceX = sourceColumn * sourceWidth;
        const float sourceY = row * sourceHeight;

        if (mirrored[index]) {
            ofPushMatrix();
            ofTranslate(destinationX + destinationWidth, destinationY);
            ofScale(-1.0f, 1.0f);
            texture.drawSubsection(
                0.0f,
                0.0f,
                destinationWidth,
                destinationHeight,
                sourceX,
                sourceY,
                sourceWidth,
                sourceHeight);
            ofPopMatrix();
        } else {
            texture.drawSubsection(
                destinationX,
                destinationY,
                destinationWidth,
                destinationHeight,
                sourceX,
                sourceY,
                sourceWidth,
                sourceHeight);
        }
    }

    ofSetLineWidth(1.0f);
    ofSetColor(255, 180);
    ofDrawLine(bounds.getCenter().x, bounds.getTop(), bounds.getCenter().x, bounds.getBottom());
    ofDrawLine(bounds.getLeft(), bounds.getCenter().y, bounds.getRight(), bounds.getCenter().y);

    for (std::size_t index = 0; index < regionCount; ++index) {
        const std::size_t column = index % 2;
        const std::size_t row = index / 2;
        const float x = bounds.getX() + column * destinationWidth;
        const float y = bounds.getY() + row * destinationHeight;

        if (mirrored[index]) {
            ofNoFill();
            ofSetLineWidth(4.0f);
            ofSetColor(0, 220, 160);
            ofDrawRectangle(x + 2.0f, y + 2.0f, destinationWidth - 4.0f, destinationHeight - 4.0f);
        }

        const std::string status = mirrored[index] ? "  MIRRORED" : "  LIVE";
        ofDrawBitmapStringHighlight(
            std::string(regionLabel(index)) + status,
            x + 10.0f,
            y + 20.0f,
            ofColor(0, 150),
            ofColor::white);
    }

    ofPopStyle();
}

void mirrorRegions::toggle(Region region) {
    const auto index = indexFor(region);
    mirrored[index] = !mirrored[index];
}

void mirrorRegions::clear() {
    mirrored.fill(false);
}

bool mirrorRegions::isMirrored(Region region) const {
    return mirrored[indexFor(region)];
}

std::size_t mirrorRegions::indexFor(Region region) {
    return static_cast<std::size_t>(region);
}
