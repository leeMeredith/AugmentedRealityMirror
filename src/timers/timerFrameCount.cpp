/*
 *  timerFrameCount.cpp
 *  freeActor_6_1_1
 *
 *  Created by Lee Meredith on 6/6/11.
 *  Copyright 2011 leelee. All rights reserved.
 *
 */

#include "timerFrameCount.h"

//--------------------------------------------------------------
void timerFrameCount::setup(int newThisLong){	 
	counter = 0;
	vagRounded.loadFont("mono.ttf", 18);
	testMe = false;
	intTest = 0;
	startTimer = false;
	
	//length = 3000;
	thisLong = newThisLong;//need to set to newThisLong + 1
	testLength = thisLong-40;//then thisLong - 2
	
	isGetFN= false;
	pbac_FN = 0;
	thisMuch = 0;
	toggleMill = false;
	
	howManyTime = 5;
	thisManyTime = 0;
	//dBug = false;
	//new_X = 0;
	//new_Y = 35;
	frameForPlayback = 0;
	setFramPlayback = 0;
}


//--------------------------------------------------------------
void timerFrameCount::update(){	
	if (testMe == true) {
		isNow = true;
		pbac_FN = 0;//ofGetFrameNum();
		isGetFN = true;
		//timer------_
		if (isGetFN == true) {
			
			if (ofGetFrameNum() > 0){
				playbackStart_FN++;	
			}
						
			frameForPlayback = playbackStart_FN % thisLong;
			if(setFramPlayback == 1){frameForPlayback = thisLong - frameForPlayback - 1;}
			
			thisMuch = (frameForPlayback-pbac_FN) % thisLong;//was 2
			
			toggleMill = !toggleMill;
			
			if (thisMuch == thisLong-1){
				thisManyTime++;
				isGetFN = false;
			}
			//startTimer = true;
		}
		//---timer---_
		
		if (isGetFN == false) {
			if (dBug == true) {
				//cout << "nameMe " << nameMe << endl;
				//cout << "isGetFN " << isGetFN << endl;
			}
		}
		if (isGetFN == true) {
			if (dBug == true) {
				//cout << "nameMe----------- " << nameMe << endl;
				//cout << "thisManyTime----------- " << thisManyTime << endl;
			}
		}
	}
	
	if (startTimer == true) {
		isGetFN = false;
		pbac_FN = 0;
		thisMuch = 0;
		playbackStart_FN = 0;
		startTimer = !startTimer;
		testMe = !testMe;
	}	
	
	if (thisManyTime >= howManyTime) {
		//thisManyTime = 0;
	}
}

//--------------------------------------------------------------
void timerFrameCount::draw(int new_X, int new_Y){
	/*
	 sprintf (timeString, "time: %0.2i:%0.2i:%0.2i \nelapsed time %i", ofGetHours(), ofGetMinutes(), ofGetSeconds(), thisManyTime);
	 
	 //float w = vagRounded.stringWidth(eventString);
	 //float h = vagRounded.stringHeight(eventString);
	 
	 ofSetColor(0xffffff);
	 vagRounded.drawString(eventString, 88,198);
	 
	 ofSetColor(255,0,0);
	 vagRounded.drawString(eventString, 90,200);
	 
	 
	 ofSetColor(0xffffff);
	 vagRounded.drawString(timeString, 88,98);
	 
	 ofSetColor(255,0,0);
	 vagRounded.drawString(timeString, 90,100);
	 */
	
	if (isNow == true) {
		ofSetColor(255,0,0);
		ofRect(ofGetWidth()/2-new_X , new_Y, 30, 30);
	}
	if (isNow == false) {
		ofSetColor(0,255,0);
		ofRect(ofGetWidth()/2-new_X , new_Y, 10, 10);
		
	}
	
	if (isGetFN == true) {
		ofSetColor(255,0,0);
		ofRect(ofGetWidth()/2+new_X , new_Y, 30, 30);
	}
	if (isGetFN == false) {
		ofSetColor(0,0,255);
		ofRect(ofGetWidth()/2+new_X , new_Y, 10, 10);
		
	}
	
}

//--------------------------------------------------------------
void timerFrameCount::setPlaybackStart_FN(int newPlaybackStart_FN){
	playbackStart_FN = newPlaybackStart_FN;
}

//--------------------------------------------------------------
void timerFrameCount::keyPressed(int key){ 
	if (key == ' ') {
		testMe = !testMe;
		isGetFN= false;
		pbac_FN = 0;
		thisMuch = 0;
	}
}
