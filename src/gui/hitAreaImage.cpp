/*
 *  hitAreaImage.cpp
 */

#include "hitAreaImage.h"

static bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}

//-------------------------------------------
bool hitAreaImage::checkHit(float x, float y){
	if(isInsideRect(x, y, boundingBox)){
		isIn = true;
	}else {
        isIn = false;
    }
}

//-----------------------------------------------
void hitAreaImage::setPosition(float x, float y){
	boundingBox.x = x;
	boundingBox.y = y;
	myHitAreaImage.x     = x;
	myHitAreaImage.y     = y;
//    setColorIn_0.set(255, 0, 0, 255);
//    setColorIn_1.set(255, 255, 255, 255);
//    setColorSel_0.set(255, 0, 255, 255);
//    setColorSel_1.set(0, 0, 0, 75);
}

//------------------------------------------------
void hitAreaImage::setDimensions(){
	myHitAreaImage.width       = myImage.width;
	myHitAreaImage.height      = myImage.height;
	boundingBox.width   = myImage.width;
	boundingBox.height  = myImage.height;
}

//--------------------------------------------------------------
void hitAreaImage::setImage(ofImage newImage){
	myImage = newImage;
}

//--------------------------------------------------------------
void hitAreaImage::draw(float x, float y){
    
    if (isSel == true) {
        ofNoFill();
        ofSetColor(setColorSel_0);
        ofRect(x-1, y-1, myImage.width+2, myImage.height+2);
    }else {
        ofNoFill();
        ofSetColor(setColorSel_1);
        ofRect(x-1, y-1, myImage.width+2, myImage.height+2);
    }
    ofFill();
    if (isIn == true) {
        ofSetColor(setColorIn_0);
    }else {
        ofSetColor(setColorIn_1);
    }
    
	boundingBox.set(x, y, myImage.width, myImage.height);
    myImage.draw(x, y);
}