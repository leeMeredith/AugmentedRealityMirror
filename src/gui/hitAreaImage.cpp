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
        ofSetColor(255, 0, 255, 255);
        ofRect(x-1, y-1, myImage.width+2, myImage.height+2);
    }else {
        ofNoFill();
        ofSetColor(0, 0, 0, 75);
        ofRect(x-1, y-1, myImage.width+2, myImage.height+2);
    }
    ofFill();
    if (isIn == true) {
        ofSetColor(255, 0, 0);
    }else {
        ofSetColor(255, 255, 255);
    }
    
	boundingBox.set(x, y, myImage.width, myImage.height);
    myImage.draw(x, y);
}