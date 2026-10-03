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
    float verticalDividerPosition,
    float horizontalDividerPosition,
    bool showOverlay) const {
    if (!texture.isAllocated() || bounds.isEmpty()) {
        return;
    }

    const float verticalDivider = ofClamp(verticalDividerPosition, 0.05f, 0.95f);
    const float horizontalDivider = ofClamp(horizontalDividerPosition, 0.05f, 0.95f);
    const float destinationLeftWidth = bounds.getWidth() * verticalDivider;
    const float destinationRightWidth = bounds.getWidth() - destinationLeftWidth;
    const float destinationTopHeight = bounds.getHeight() * horizontalDivider;
    const float destinationBottomHeight = bounds.getHeight() - destinationTopHeight;
    const float sourceLeftWidth = texture.getWidth() * verticalDivider;
    const float sourceRightWidth = texture.getWidth() - sourceLeftWidth;
    const float sourceTopHeight = texture.getHeight() * horizontalDivider;
    const float sourceBottomHeight = texture.getHeight() - sourceTopHeight;
    const float dividerX = bounds.getX() + destinationLeftWidth;
    const float dividerY = bounds.getY() + destinationTopHeight;

    ofPushStyle();
    ofSetColor(255);

    for (std::size_t index = 0; index < regionCount; ++index) {
        const std::size_t column = index % 2;
        const std::size_t row = index / 2;
        const bool useBaseMirror = !mirrored[index];
        const float destinationWidth = column == 0
            ? destinationLeftWidth
            : destinationRightWidth;
        const float destinationHeight = row == 0
            ? destinationTopHeight
            : destinationBottomHeight;
        const float destinationX = column == 0 ? bounds.getX() : dividerX;
        const float destinationY = row == 0 ? bounds.getY() : dividerY;
        const float sourceWidth = column == 0 ? sourceLeftWidth : sourceRightWidth;
        const float sourceHeight = row == 0 ? sourceTopHeight : sourceBottomHeight;
        const float sourceY = row == 0 ? 0.0f : sourceTopHeight;
        const float sourceX = useBaseMirror
            ? (column == 0 ? texture.getWidth() - sourceWidth : 0.0f)
            : (column == 0 ? 0.0f : sourceLeftWidth);

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
    ofDrawLine(dividerX, bounds.getTop(), dividerX, bounds.getBottom());
    ofDrawLine(bounds.getLeft(), dividerY, bounds.getRight(), dividerY);

    for (std::size_t index = 0; index < regionCount; ++index) {
        const std::size_t column = index % 2;
        const std::size_t row = index / 2;
        const float destinationWidth = column == 0
            ? destinationLeftWidth
            : destinationRightWidth;
        const float destinationHeight = row == 0
            ? destinationTopHeight
            : destinationBottomHeight;
        const float x = column == 0 ? bounds.getX() : dividerX;
        const float y = row == 0 ? bounds.getY() : dividerY;

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
