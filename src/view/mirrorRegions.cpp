#include "mirrorRegions.h"

namespace {
constexpr std::size_t regionCount = 4;

const char* regionLabel(std::size_t index) {
    static constexpr std::array<const char*, regionCount> labels{
        "Q  TOP LEFT",
        "W  TOP RIGHT",
        "A  BOTTOM LEFT",
        "S  BOTTOM RIGHT"
    };
    return labels.at(index);
}
}

void mirrorRegions::draw(
    const ofTexture& texture,
    const ofRectangle& bounds,
    const Selection& mirrored,
    bool showOverlay) const {
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
        const bool useBaseMirror = !mirrored[index];
        const std::size_t sourceColumn = useBaseMirror ? 1 - column : column;

        const float destinationX = bounds.getX() + column * destinationWidth;
        const float destinationY = bounds.getY() + row * destinationHeight;
        const float sourceX = sourceColumn * sourceWidth;
        const float sourceY = row * sourceHeight;

        if (useBaseMirror) {
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

    if (!showOverlay) {
        ofPopStyle();
        return;
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

        const std::string status = mirrored[index] ? "  OPPOSITE COPY" : "  BASE MIRROR";
        ofDrawBitmapStringHighlight(
            std::string(regionLabel(index)) + status,
            x + 10.0f,
            y + 20.0f,
            ofColor(0, 150),
            ofColor::white);
    }

    ofPopStyle();
}
