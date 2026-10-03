#include "splitScreenView.h"

namespace {
void drawOriginal(
    const ofTexture& texture,
    float destinationX,
    float destinationY,
    float destinationWidth,
    float destinationHeight,
    float sourceX,
    float sourceWidth) {
    texture.drawSubsection(
        destinationX,
        destinationY,
        destinationWidth,
        destinationHeight,
        sourceX,
        0.0f,
        sourceWidth,
        texture.getHeight());
}

void drawMirrored(
    const ofTexture& texture,
    float destinationX,
    float destinationY,
    float destinationWidth,
    float destinationHeight,
    float sourceX,
    float sourceWidth) {
    ofPushMatrix();
    ofTranslate(destinationX + destinationWidth, destinationY);
    ofScale(-1.0f, 1.0f);
    texture.drawSubsection(
        0.0f,
        0.0f,
        destinationWidth,
        destinationHeight,
        sourceX,
        0.0f,
        sourceWidth,
        texture.getHeight());
    ofPopMatrix();
}
}

void splitScreenView::draw(
    const ofTexture& texture,
    const ofRectangle& bounds,
    const viewConfiguration& configuration,
    bool showOverlay) const {
    if (!texture.isAllocated() || bounds.isEmpty()) {
        return;
    }

    const float divider = ofClamp(configuration.dividerPosition, 0.05f, 0.95f);
    const float leftWidth = bounds.getWidth() * divider;
    const float rightWidth = bounds.getWidth() - leftWidth;
    const float leftSourceWidth = texture.getWidth() * divider;
    const float rightSourceWidth = texture.getWidth() - leftSourceWidth;
    const float dividerX = bounds.getX() + leftWidth;

    ofPushStyle();
    ofSetColor(255);

    if (configuration.mirrorOnLeft) {
        drawMirrored(
            texture,
            bounds.getX(),
            bounds.getY(),
            leftWidth,
            bounds.getHeight(),
            rightSourceWidth,
            leftSourceWidth);
        drawOriginal(
            texture,
            dividerX,
            bounds.getY(),
            rightWidth,
            bounds.getHeight(),
            leftSourceWidth,
            rightSourceWidth);
    } else {
        drawOriginal(
            texture,
            bounds.getX(),
            bounds.getY(),
            leftWidth,
            bounds.getHeight(),
            0.0f,
            leftSourceWidth);
        drawMirrored(
            texture,
            dividerX,
            bounds.getY(),
            rightWidth,
            bounds.getHeight(),
            0.0f,
            rightSourceWidth);
    }

    ofSetColor(255, 210);
    ofSetLineWidth(2.0f);
    ofDrawLine(dividerX, bounds.getTop(), dividerX, bounds.getBottom());

    if (showOverlay) {
        const std::string leftLabel = configuration.mirrorOnLeft ? "MIRROR" : "ORIGINAL";
        const std::string rightLabel = configuration.mirrorOnLeft ? "ORIGINAL" : "MIRROR";
        ofDrawBitmapStringHighlight(
            leftLabel,
            bounds.getX() + 10.0f,
            bounds.getY() + 20.0f,
            ofColor(0, 150),
            ofColor::white);
        ofDrawBitmapStringHighlight(
            rightLabel,
            dividerX + 10.0f,
            bounds.getY() + 20.0f,
            ofColor(0, 150),
            ofColor::white);
    }

    ofPopStyle();
}
