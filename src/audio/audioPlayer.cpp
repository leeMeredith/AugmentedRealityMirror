//
//  Created by Lee Meredith.
//

#include "audioPlayer.h"

//--------------------------------------------------------------
void audioPlayer::setup(int newRectX, int newRectY, string newLargeAudioDir){
    rectX = newRectX;
	rectY = newRectY;
    largeAudioDirTest = largeAudioDir = newLargeAudioDir;
    //xml----------------------------_
    setMessageDir = "record/message/message_presets_ARM.xml";
    if( getXML.loadFile(setMessageDir) ){
        cout << "message_presets.xml loaded from documents folder!" << endl;
    }else{
        cout << "unable to load message_presets.xml check data/ folder" << endl;
    }
    audio = play = paused = volume = speed = position = "";
    getMessAudioXML[0] = getXML.getValue("AUDIO:message_0" , "XML?");
    getMessAudioXML[1] = getXML.getValue("AUDIO:message_1" , "XML?");
    getMessAudioXML[2] = getXML.getValue("AUDIO:message_2" , "XML?");
    getMessAudioXML[3] = getXML.getValue("AUDIO:message_3" , "XML?");
    getMessAudioXML[4] = getXML.getValue("AUDIO:message_4" , "XML?");
    getMessAudioXML[5] = getXML.getValue("AUDIO:message_5" , "XML?");
    //-------------xml---------------_
    
	knobFont.loadFont("mono.ttf", 12, false);
	guiText_0.setup();
	guiText_0.setFont(knobFont);
	guiText_0.setText(getMessAudioXML[5]);
	rectW = guiText_0.getTextWidth();
	rectH = guiText_0.getTextHeight();
    
    
	large.loadSound(largeAudioDir);
	large.setVolume(0.75f);
	large.setMultiPlay(false);
    
    isPlay = isPaused = false;
    
    getLargePositionMS = large.getPositionMS();
    getLargePosition = large.getPosition();// just to have a 0-1 Position
    getLargeSpeed = large.getSpeed();
    getLargeVolume = large.getVolume();
    
    //ofxSimpleSlider-----------------_
    playColor.set(255, 255, 255, 255);
    pausedColor.set(0, 0, 255, 255);
    controlColor.set(255, 0, 255, 255);
    //----------ofxSimpleSlider-------_

}

//--------------------------------------------------------------
void audioPlayer::update(int newRectX, int newRectY, string newLargeAudioDir){
    largeAudioDir = newLargeAudioDir;
    
    if (largeAudioDir != largeAudioDirTest) {
        large.unloadSound();
        large.loadSound(largeAudioDir);
        largeAudioDirTest = largeAudioDir;
    }
    
    //ofxSimpleSlider-----------------_
    float pctPos = 0;//sliderPosition.getValue();
    float pctSpeed = 1;//sliderSpeed.getValue();
    float pctVolume = 0.75;//sliderVolume.getValue();
    largeAudioPosition = pctPos;
    largeAudioSpeed = pctSpeed;
    largeAudioVolume = pctVolume;
    //----------ofxSimpleSlider-------_

    
    
    getLargePositionMS = large.getPositionMS();
    getLargePosition = large.getPosition();// just to have a 0-1 Position
    getLargeSpeed = large.getSpeed();
    getLargeVolume = large.getVolume();
    
	if (isPlay == true) {
        large.play();
        //large.setPositionMS(largeAudioPosition);
        large.setPosition(largeAudioPosition);
		large.setSpeed(largeAudioSpeed);
        large.setVolume(largeAudioVolume);
        isPlay = false;
    }
    if (large.getIsPlaying()) {
        playColor.set(255, 0, 255, 255);
    }else {
        playColor.set(100, 100, 255, 255);
    }
    if (isPaused == true) {
        large.setPaused(true);
        pausedColor.set(255, 0, 255, 255);
        isPlay = false;
    }else {
        pausedColor.set(100, 100, 255, 255);
        large.setPaused(false);
    }
   	// update the sound playing system:
    ofSoundUpdate();

}

//--------------------------------------------------------------
void audioPlayer::draw(){
    ofFill();
    ofEnableAlphaBlending();
	ofSetColor(0, 0, 255, 40);
    int newRectH = rectH*3;
	ofRect(rectX-12, rectY-100, rectW+43, newRectH);
    
    ofNoFill();
	ofSetColor(0, 0, 255, 255);
	ofRect(rectX-12, rectY-100, rectW+43, newRectH);

    ofFill();
    ofSetColor(100, 100, 255, 255);
	guiText_0.renderString(getMessAudioXML[0], rectX-10, rectY-85);
    ofSetColor(playColor);
	guiText_0.renderString(getMessAudioXML[1], rectX-10, rectY-65);
    ofSetColor(pausedColor);
	guiText_0.renderString(getMessAudioXML[2], rectX+50, rectY-65);
    //ofSetColor(controlColor);
	//guiText_0.renderString(getMessAudioXML[3], rectX-10, rectY-45);
	//guiText_0.renderString(getMessAudioXML[4], rectX-10, rectY-25);
    //guiText_0.renderString(getMessAudioXML[5], rectX-10, rectY-5);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void audioPlayer::keyPressed  (int key){
    if(key == 'p'){
        isPlay = true;
        //newGetLargePositionMS = large.getPositionMS();
    }
    if(key == 'q'){
        isPaused = !isPaused;
        isPlay = true;
        newGetLargePositionMS = large.getPositionMS();
    }
}

//--------------------------------------------------------------
void audioPlayer::keyReleased(int key){

}

//--------------------------------------------------------------
void audioPlayer::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void audioPlayer::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void audioPlayer::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void audioPlayer::mouseReleased(int x, int y, int button){

}