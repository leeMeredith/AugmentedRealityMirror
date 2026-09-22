/*
 *  playbackHitAreaImage.h
 */

#pragma once

#include "ofMain.h"
#include "hitAreaImage.h"
#include "guiText.h"

#define MAX_NUM_IMAGE 4

class playbackHitAreaImage{
	
public:
	
    void setup();
	bool checkHit(float new_x, float new_y);
	void setPosition(float new_x, float new_y);
	void setDimensions();
    void setImage(ofImage newImage[MAX_NUM_IMAGE]);
    
	void draw(float new_x, float new_y);
	void mousePressed(int x, int y, int button);
    
    float boundingBoxWidth, boundingBoxHeight;
    float pressedSel_x, pressedSel_y;
    float addPosX, testAddPosX;
    bool isSel;
    float allSize;
    float test_x, test_y;
    int hitAreaImageIndex;
    
    int getDialogIndex;
    int allHitAdd;
    bool isHitOk, isImageWidthNew;
    bool isIn[MAX_NUM_IMAGE];
    string hitDialogIndex;
    string dialogIndex[MAX_NUM_IMAGE];
    ofColor testSelPPMode_0, testInPPMode_0;
    ofColor testSelPPMode_1, testInPPMode_1;
    ofColor testSelRecMode_0, testInRecMode_0;
    ofColor testSelRecMode_1, testInRecMode_1;
    ofColor setColorSel_0[MAX_NUM_IMAGE], setColorSel_1[MAX_NUM_IMAGE];
    ofColor setColorIn_0[MAX_NUM_IMAGE], setColorIn_1[MAX_NUM_IMAGE];
    hitAreaImage hitAreaImage_0[MAX_NUM_IMAGE];
    
    //guiText-----_
    guiText guiText_0;
    ofTrueTypeFont myFont;
    //---guiText--_
    
};