/*
 *  timerFrameCount.h
 *  freeActor_6_1_1
 *
 *  Created by Lee Meredith on 6/6/11.
 *  Copyright 2011 leelee. All rights reserved.
 *
 */

#ifndef TIMER_FN_COUNT
#define TIMER_FN_COUNT


#include "ofMain.h"

class timerFrameCount{
	
public:
	
	void setup(int newThisLong);
	void update();
	void draw(int new_X, int new_Y);
	void setPlaybackStart_FN(int newPlaybackStart_FN);
	void keyPressed(int key);
	
	float 			counter;
	ofTrueTypeFont 	vagRounded;
	char eventString[255];
	char timeString[255];
	bool testMe;
	int intTest;
	
	int length, testLength, thisLong, isNow;
	int pbac_FN, isGetFN, thisMuch;
	bool startTimer, toggleMill; 
	int millAtStart;
	int	howManyTime, thisManyTime;
	int nameMe;
	bool dBug;
	
	bool setFramPlayback;
	int	frameForPlayback;
	int playbackStart_FN;
};

#endif	
