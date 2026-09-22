//
//  Created by Lee Meredith.
//

#include "velSmoothState.h"

//--------------------------------------------------------------
void velSmoothState::setup(int newMinStoppedFrames, int newMinMovingFrames){
	
	prev = curr;
	prevVel = 0;
	
	//ofSetFrameRate(60);
	
	stoppedFrameCount = 0;
	movingFrameCount = 0;
	bStopped = false;
	minStoppedFrames = newMinStoppedFrames;
	minMovingFrames = newMinMovingFrames;
	
	pos = 0;
	
	dX = 0;
	dY = 0;
	
}

//--------------------------------------------------------------
void velSmoothState::update(float newPos){
	
	pos = newPos;
	curr = 0.9f * curr + 0.1f * pos;
	vel = curr - prev;
	acc = 0.9f * acc + 0.1f * fabs(vel - prevVel);
	prevVel = vel;
	prev = curr;
	
	if (fabs(vel) > 0.08){
		movingFrameCount++;
		stoppedFrameCount = 0;
	} else {
		movingFrameCount = 0;
		stoppedFrameCount++;
	}
	
	if (stoppedFrameCount > minStoppedFrames){
		bStopped = true;
	}
	
	if (movingFrameCount > minMovingFrames){
		bStopped = false;
	}
	
}

//--------------------------------------------------------------
void velSmoothState::draw(float newX, float newY){
	dX = newX;
	dY = newY; 
	
	if (bStopped == false){
		ofSetColor(0,255,0);
	}
	if (bStopped == true){
		ofSetColor(255,0,0);
	}	
	//ofRect(dX,dY,5,5);
	
	ofDrawBitmapString("vel = " + ofToString(vel, 3), dX, dY+10);//velocity
	ofDrawBitmapString("acc = " + ofToString(acc, 3), dX, dY+30);//acceleration
	ofDrawBitmapString("moving " + ofToString(movingFrameCount, 0), dX, dY+50);//movingFrameCount
	ofDrawBitmapString("stopped " + ofToString(stoppedFrameCount, 0), dX, dY+70);//stoppedFrameCount
}

//--------------------------------------------------------------
bool velSmoothState::bStop(){
	return bStopped;
}

//--------------------------------------------------------------
float velSmoothState::velocityl(){
	return vel;
}

//--------------------------------------------------------------
float velSmoothState::accel(){
	return acc;
}