#include "dialogPlayer.h"

//--------------------------------------------------------------
void dialogPlayer::setup(int newRectX, int newRectY, string newDialogAudioDir){
    rectX = newRectX;
	rectY = newRectY;
    dialogAudioDirTest = dialogAudioDir = newDialogAudioDir;
    //xml----------------------------_
    setMessageDir = "record/message/message_presets_ARM.xml";
    if( getXML.loadFile(setMessageDir) ){
        cout << "message_presets.xml loaded from documents folder!" << endl;
    }else if( getXML.loadFile("record/message/message_presets_ARM.xml") ){
        cout << "message_presets.xml loaded from data folder!" << endl;
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
    
    
	dialog.loadSound(dialogAudioDir);
	dialog.setVolume(0.75f);
	dialog.setMultiPlay(false);
    
    isPlay = isPaused = false;
    
    getDialogPositionMS = dialog.getPositionMS();
    getDialogPosition = dialog.getPosition();// just to have a 0-1 Position
    getDialogSpeed = dialog.getSpeed();
    getDialogVolume = dialog.getVolume();
    
    //ofxSimpleSlider-----------------_
    playColor.set(255, 255, 255, 255);
    pausedColor.set(0, 0, 255, 255);
    controlColor.set(255, 0, 255, 255);
    sliderVolume.setup(150, 320+20, 100, 20, 0, 1, 0.75f, false, 0);
    sliderSpeed.setup(150, 320+40, 100, 20, 0, 1, 1, false, 0);
    sliderPosition.setup(150, 320+60, 100, 20, 0, 1, 0, false, 0);
    //----------ofxSimpleSlider-------_

}

//--------------------------------------------------------------
void dialogPlayer::update(int newRectX, int newRectY, string newDialogAudioDir){
    dialogAudioDir = newDialogAudioDir;
    
    if (dialogAudioDir != dialogAudioDirTest) {
        dialog.unloadSound();
        dialog.loadSound(dialogAudioDir);
        dialogAudioDirTest = dialogAudioDir;
    }
    
    //ofxSimpleSlider-----------------_
    float pctPos = sliderPosition.getValue();
    float pctSpeed = sliderSpeed.getValue();
    float pctVolume = sliderVolume.getValue();
    dialogAudioPosition = pctPos;
    dialogAudioSpeed = pctSpeed;
    dialogAudioVolume = pctVolume;
    //----------ofxSimpleSlider-------_

    
    
    getDialogPositionMS = dialog.getPositionMS();
    getDialogPosition = dialog.getPosition();// just to have a 0-1 Position
    getDialogSpeed = dialog.getSpeed();
    getDialogVolume = dialog.getVolume();
    
	if (isPlay == true) {
        dialog.play();
        //dialog.setPositionMS(dialogAudioPosition);
        dialog.setPosition(dialogAudioPosition);
		dialog.setSpeed(dialogAudioSpeed);
        dialog.setVolume(dialogAudioVolume);
        isPlay = false;
    }
    if (dialog.getIsPlaying()) {
        playColor.set(255, 0, 255, 255);
    }else {
        playColor.set(100, 100, 255, 255);
    }
    if (isPaused == true) {
        dialog.setPaused(true);
        pausedColor.set(255, 0, 255, 255);
        isPlay = false;
    }else {
        pausedColor.set(100, 100, 255, 255);
        dialog.setPaused(false);
    }
   	// update the sound playing system:
    ofSoundUpdate();

}

//--------------------------------------------------------------
void dialogPlayer::draw(){
    ofFill();
    ofEnableAlphaBlending();
	ofSetColor(0, 0, 255, 40);
    int newRectH = rectH*7+9;
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
    ofSetColor(controlColor);
	guiText_0.renderString(getMessAudioXML[3], rectX-10, rectY-45);
	guiText_0.renderString(getMessAudioXML[4], rectX-10, rectY-25);
    guiText_0.renderString(getMessAudioXML[5], rectX-10, rectY-5);
}

//--------------------------------------------------------------
void dialogPlayer::keyPressed  (int key){
    if(key == 'p'){
        isPlay = true;
        //newGetDialogPositionMS = dialog.getPositionMS();
    }
    if(key == 'P'){
        isPaused = !isPaused;
        isPlay = true;
        newGetDialogPositionMS = dialog.getPositionMS();
    }
}

//--------------------------------------------------------------
void dialogPlayer::keyReleased(int key){

}

//--------------------------------------------------------------
void dialogPlayer::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void dialogPlayer::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void dialogPlayer::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void dialogPlayer::mouseReleased(int x, int y, int button){

}