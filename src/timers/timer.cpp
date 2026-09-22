/*
 *  timer.cpp
 *  freeActor_6_1_1
 *
 *  Created by Lee Meredith on 6/6/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "timer.h"

//--------------------------------------------------------------
void timer::setup(int newThisLongTop, int newThisLongInSide){
	
	thisLongTop = newThisLongTop;
	thisLongInSide = newThisLongInSide;
	
	howManyTimeTop = 4;// = newHowManyTimeTop;
	howManyTimeInSide = 5;// = newHowManyTimeInSide;
	//to size rite need 
	//???if(howManyTimeTop < howManyTimeInSide){howManyTimeTop+howManyTimeInSide}???

	topTimer.nameMe = 1;
	topTimer.dBug = false;
	topTimer.setup(thisLongTop);// 60 ? 120
	topTimer.howManyTime = howManyTimeTop;// 4
	
	topTog.nameMe = 2;
	topTog.dBug = false;
	topTog.setup(2);// 2
	topTog.howManyTime = 1;// 5\
	
	inSideTimer.nameMe = 3;
	inSideTimer.dBug = false;
	inSideTimer.setup(thisLongInSide);// 2
	inSideTimer.howManyTime = howManyTimeInSide;// 5
	
	isThis_0 = isThis_1 = true;
	
	isAtStart = true;
	oneStart = endTimer = inStart = inEnd = false;
	
}


//--------------------------------------------------------------
void timer::update(){

	topTimer.thisLong = thisLongTop;
	topTimer.howManyTime = howManyTimeTop;
	
	if (topTimer.isGetFN == false && topTimer.thisManyTime == howManyTimeTop) {
		topTimer.thisManyTime = 0;
		topTimer.startTimer = true; 
		endTimer = true;
		isAtStart = true;
	}else {
		endTimer = false;
	}

	
	if (topTimer.isGetFN == false && topTimer.thisManyTime > 0) {
		topTog.startTimer = true;
		isAtStart = false;
	}
	if (topTog.isGetFN == false && topTog.thisManyTime > 1 ) {
		topTog.thisManyTime = 0;
		topTog.startTimer = true;
	}
	
	if (topTog.isGetFN == true && topTog.thisManyTime == 1) {
		inSideTimer.startTimer = true;	
	}
	if (topTog.isGetFN == false && topTog.thisManyTime == 1) {
		//oneStart = true;
	} else {
		oneStart = false;
	}
	
	if (inSideTimer.isGetFN == true && inSideTimer.thisManyTime == 1 && isThis_0 == true ) {
		inEnd = true;
	} else {
		inEnd = false;
	}

	if (inSideTimer.isGetFN == true && inSideTimer.thisManyTime == 4 && isThis_1 == true ) {
		inStart = true;
	} else {
		inStart = false;
	}	
	if (inSideTimer.isGetFN == false && inSideTimer.thisManyTime == 5 ) {
		inSideTimer.thisManyTime = 0;
		inSideTimer.startTimer = true;
	}
	
	topTimer.update();
	topTog.update();
	inSideTimer.update();
	
	
}

//--------------------------------------------------------------
void timer::draw(int new_X, int new_Y){
	d_x = new_X;
	d_y = new_Y;
	ofFill();
	topTimer.draw(d_x+70, d_y);
	topTog.draw(d_x+40, d_y);
	inSideTimer.draw(d_x, d_y);
}


//--------------------------------------------------------------
void timer::keyPressed(int key){ 
	if (key == ' ') {
	}
}
