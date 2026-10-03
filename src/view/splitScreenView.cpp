#include "splitScreenView.h"

namespace {
void drawOriginal(
    const ofTexture& texture,
    const ofRectangle& destination,
    const ofRectangle& source) {
    texture.drawSubsection(
        destination.getX(),
        destination.getY(),
        destination.getWidth(),
        destination.getHeight(),
        source.getX(),
        source.getY(),
        source.getWidth(),
        source.getHeight());
}

void drawMirrored(
    const ofTexture& texture,
    const ofRectangle& destination,
    const ofRectangle& source) {
    ofPushMatrix();
    ofTranslate(destination.getRight(), destination.getY());
    ofScale(-1.0f, 1.0f);
    texture.drawSubsection(
        0.0f,
        0.0f,
        destination.getWidth(),
        destination.getHeight(),
        source.getX(),
        source.getY(),
        source.getWidth(),
        source.getHeight());
    ofPopMatrix();
}

void drawLabel(const std::string& label, float x, float y) {
    ofDrawBitmapStringHighlight(
        label,
        x,
        y,
        ofColor(0, 150),
        ofColor::white);
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

    ofPushStyle();
    ofSetColor(255);

    if (configuration.splitDirection == splitOrientation::horizontal) {
        const float divider = ofClamp(
            configuration.horizontalDividerPosition,
            0.05f,
            0.95f);
        const float topHeight = bounds.getHeight() * divider;
        const float bottomHeight = bounds.getHeight() - topHeight;
        const float sourceTopHeight = texture.getHeight() * divider;
        const float sourceBottomHeight = texture.getHeight() - sourceTopHeight;
        const float dividerY = bounds.getY() + topHeight;

        const ofRectangle topDestination(
            bounds.getX(), bounds.getY(), bounds.getWidth(), topHeight);
        const ofRectangle bottomDestination(
            bounds.getX(), dividerY, bounds.getWidth(), bottomHeight);
        const ofRectangle topSource(
            0.0f, 0.0f, texture.getWidth(), sourceTopHeight);
        const ofRectangle bottomSource(
            0.0f, sourceTopHeight, texture.getWidth(), sourceBottomHeight);

        if (configuration.mirrorOnFirstSide) {
            drawMirrored(texture, topDestination, topSource);
            drawOriginal(texture, bottomDestination, bottomSource);
        } else {
            drawOriginal(texture, topDestination, topSource);
            drawMirrored(texture, bottomDestination, bottomSource);
        }

        ofSetColor(255, 210);
        ofSetLineWidth(2.0f);
        ofDrawLine(bounds.getLeft(), dividerY, bounds.getRight(), dividerY);

        if (showOverlay) {
            drawLabel(
                configuration.mirrorOnFirstSide ? "MIRROR" : "ORIGINAL",
                bounds.getX() + 10.0f,
                bounds.getY() + 20.0f);
            drawLabel(
                configuration.mirrorOnFirstSide ? "ORIGINAL" : "MIRROR",
                bounds.getX() + 10.0f,
                dividerY + 20.0f);
        }
    } else {
        const float divider = ofClamp(
            configuration.verticalDividerPosition,
            0.05f,
            0.95f);
        const float leftWidth = bounds.getWidth() * divider;
        const float rightWidth = bounds.getWidth() - leftWidth;
        const float sourceLeftWidth = texture.getWidth() * divider;
        const float sourceRightWidth = texture.getWidth() - sourceLeftWidth;
        const float dividerX = bounds.getX() + leftWidth;

        const ofRectangle leftDestination(
            bounds.getX(), bounds.getY(), leftWidth, bounds.getHeight());
        const ofRectangle rightDestination(
            dividerX, bounds.getY(), rightWidth, bounds.getHeight());
        const ofRectangle leftSource(
            0.0f, 0.0f, sourceLeftWidth, texture.getHeight());
        const ofRectangle rightSource(
            sourceLeftWidth, 0.0f, sourceRightWidth, texture.getHeight());
        const ofRectangle sourceForLeftMirror(
            sourceRightWidth, 0.0f, sourceLeftWidth, texture.getHeight());
        const ofRectangle sourceForRightMirror(
            0.0f, 0.0f, sourceRightWidth, texture.getHeight());

        if (configuration.mirrorOnFirstSide) {
            drawMirrored(texture, leftDestination, sourceForLeftMirror);
            drawOriginal(texture, rightDestination, rightSource);
        } else {
            drawOriginal(texture, leftDestination, leftSource);
            drawMirrored(texture, rightDestination, sourceForRightMirror);
        }

        ofSetColor(255, 210);
        ofSetLineWidth(2.0f);
        ofDrawLine(dividerX, bounds.getTop(), dividerX, bounds.getBottom());

        if (showOverlay) {
            drawLabel(
                configuration.mirrorOnFirstSide ? "MIRROR" : "ORIGINAL",
                bounds.getX() + 10.0f,
                bounds.getY() + 20.0f);
            drawLabel(
                configuration.mirrorOnFirstSide ? "ORIGINAL" : "MIRROR",
                dividerX + 10.0f,
                bounds.getY() + 20.0f);
        }
    }

    ofPopStyle();
}
