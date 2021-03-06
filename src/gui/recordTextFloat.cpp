//
//  Created by Lee Meredith.
//

#include "recordTextFloat.h"

//--------------------------------------------------------------
void recordTextFloat::setup(float newRectX, float newRectY){
	score = 0;
    char tempScoreString[255];
    sprintf(tempScoreString,"%f", score);
    newScoreString = tempScoreString;
    string setSome = "000";
    rectX = newRectX;
	rectY = newRectY;
	scoreFont.loadFont("mono.ttf", 12, false);
	guiText_0.setup();
	guiText_0.setFont(scoreFont);
	guiText_0.setText(newScoreString);
	rectW = guiText_0.getTextWidth();
	rectH = guiText_0.getTextHeight();
	
}

//--------------------------------------------------------------
void recordTextFloat::update(float newRectX, float newRectY){
    rectX = newRectX;
	rectY = newRectY;
	char tempScoreString[255];
    sprintf(tempScoreString,"%f", score);
    newScoreString = tempScoreString;
    guiText_0.setText(newScoreString);
}

//--------------------------------------------------------------
void recordTextFloat::draw(){
    ofEnableAlphaBlending();
    ofSetColor(scoreColor);
    ofPushMatrix();
    ofTranslate((rectX-11)+guiText_0.bounds.width/2, rectY+guiText_0.bounds.height/2, 0);
    ofScale(1.0 + scaleX, 1.0 + scaleY, 1.0);
    guiText_0.renderString(newScoreString, -guiText_0.bounds.width/2, guiText_0.bounds.height/2);
    ofPopMatrix();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void recordTextFloat::setScale(float newX, float newY, float newZ){
    scaleX = newX;
    scaleY = newY;
    scaleZ = newZ;
}


//--------------------------------------------------------------
void recordTextFloat::textColor(ofColor newScoreColor){
    scoreColor = newScoreColor;
}

//--------------------------------------------------------------
void recordTextFloat::rectColor(ofColor newRectColor){
    rectColorBG = newRectColor;
}