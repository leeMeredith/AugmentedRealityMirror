//
//  Created by Lee Meredith.
//

#include "velSmoothStateXY.h"

//--------------------------------------------------------------
void velSmoothStateXY::setup(int newMinStoppedFrames, int newMinMovingFrames){
	//ofSetFrameRate(60);
	//state--------------------
	stateX.setup(minStoppedFrames, minMovingFrames);
	stateY.setup(minStoppedFrames, minMovingFrames);
	//-----------state---------
	
	stoppedFrameCountX = stoppedFrameCountY = 0;
	movingFrameCountX = movingFrameCountY = 0;
	bStoppedX = bStoppedY = false;
	minStoppedFrames = newMinStoppedFrames;
	minMovingFrames = newMinMovingFrames;
	
	posX = posY = 0;
	accX = accY = 0;
	
	dX = 0;
	dY = 0;
	
}

//--------------------------------------------------------------
void velSmoothStateXY::update(float newPosX, float newPosY){
	posX = newPosX;
	posY = newPosY;	
	stateX.update(posX);
	stateY.update(posY);
	bStoppedX = stateX.bStopped;
	bStoppedY = stateY.bStopped;
	velX = stateX.vel;
	velY = stateY.vel;
	accX = stateX.acc;
	accY = stateY.acc;
	stoppedFrameCountX = stateX.stoppedFrameCount;
	stoppedFrameCountY = stateY.stoppedFrameCount;
	movingFrameCountX = stateX.movingFrameCount;
	movingFrameCountY = stateY.movingFrameCount;
}

//--------------------------------------------------------------
void velSmoothStateXY::draw(float newX, float newY){
	dX = newX;
	dY = newY; 
	stateX.draw(dX, dY);
	stateY.draw(dX+100, dY);
}