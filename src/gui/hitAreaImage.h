/*
 *  hitAreaImage.h
 */

#pragma once

#include "ofMain.h"

#define SG_STATE_SELECTED 1

class hitAreaImage{
	
public:
	
	bool checkHit(float x, float y);
	void setPosition(float x, float y);
	void setDimensions();
    void setImage(ofImage newImage);
    
	void draw(float x, float y);
	
	ofRectangle boundingBox;
	ofRectangle myHitAreaImage;
    
	float offsetX, offsetY;
	float titleSpacing;
	float fontSize;
	bool readOnly;
	bool bShowText;
	int  state;
    bool isIn, isSel;
    ofColor setColorSel_0, setColorSel_1;
    ofColor setColorIn_0, setColorIn_1;
    ofImage myImage;
};