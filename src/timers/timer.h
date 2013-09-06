/*
 *  timer.h
 *  freeActor_6_1_1
 *
 *  Created by Lee Meredith on 6/6/11.
 *  Copyright 2011 leelee. All rights reserved.
 *
 */


#ifndef _TIMER_
#define _TIMER_


#include "ofMain.h"
#include "timerFrameCount.h"

class timer{
	
public:
	
	void setup(int newThisLongTop, int newThisLongInSide);
	void update();
	void draw(int new_X, int new_Y);
	void keyPressed(int key);
	
	float 			counter;
	
	timerFrameCount topTimer;
	timerFrameCount topTog;
	timerFrameCount inSideTimer;
	
	bool isThis_0, isThis_1;
	int thisLongTop, thisLongInSide;
	int howManyTimeTop, newHowManyTimeTop;
	int howManyTimeInSide, newHowManyTimeInSide;
	int d_x, d_y;
	int inSideManyTime;
	bool isTopTimer;
	bool isAtStart, oneStart, endTimer, inStart, inEnd;
	
};

#endif	
