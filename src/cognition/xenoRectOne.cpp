#include "xenoRectOne.h"

//------------------------------------------------------------------
xenoRectOne::xenoRectOne(){
	catchUpSpeed = 0.06f;
	
	one = 0.0f;
	prevOne = 0.0f;
}

//------------------------------------------------------------------
void xenoRectOne::draw() {
	ofFill();
	
	ofSetRectMode(OF_RECTMODE_CENTER); // center around the position
    ofSetColor(r0,g0,b0,100);
		ofRect( one,20, 30,30);
		ofNoFill();
		ofSetColor(r0,g0,b0,210);
		ofRect( one,0, 30,30);	
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//------------------------------------------------------------------
void xenoRectOne::xenoToOne(float catchOne){
	one = catchUpSpeed * catchOne + (1-catchUpSpeed) * one; 

    dOne = one - prevOne;
	prevOne = one;
}

//------------------------------------------------------------------
void xenoRectOne::setColor(int newR, int newG, int newB){
	r0 = newR;
	g0 = newG; 
	b0 = newB;
}