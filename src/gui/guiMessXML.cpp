//
//  Created by Lee Meredith.
//

#include "guiMessXML.h"

//--------------------------------------------------------------
void guiMessXML::setup(float newRectX, float newRectY, string newSetDocDir){
	
	setDocDir = newSetDocDir;//message_presets.xml;
	message = "message_presets_ARM.xml";
	if( getXML.loadFile(setDocDir) ){
		cout << "message_presets.xml loaded from documents folder!" << endl;
	}else{
		cout << "unable to load message_presets_ARM.xml check data/ folder" << endl;
	}
    maxNumInList = 7;
    getMessRecordXML[0] = getXML.getValue("RECORD:message_0" , "XML?");
	getMessRecordXML[1] = getXML.getValue("RECORD:message_1" , "XML?");
	getMessRecordXML[2] = getXML.getValue("RECORD:message_2" , "XML?");
	getMessRecordXML[3] = getXML.getValue("RECORD:message_3" , "XML?");
	getMessRecordXML[4] = getXML.getValue("RECORD:message_4" , "XML?");
	getMessRecordXML[5] = getXML.getValue("RECORD:message_5" , "XML?");
   	getMessRecordXML[6] = getXML.getValue("RECORD:message_6" , "XML?");
    
    getMessRecordXMLTags[0] = getMessRecordXMLTag = getXML.getValue("REALTAGRECORD:message_0" , "XML?");
	getMessRecordXMLTags[1] = getXML.getValue("REALTAGRECORD:message_1" , "XML?");
	getMessRecordXMLTags[2] = getXML.getValue("REALTAGRECORD:message_2" , "XML?");
	getMessRecordXMLTags[3] = getXML.getValue("REALTAGRECORD:message_3" , "XML?");
	getMessRecordXMLTags[4] = getXML.getValue("REALTAGRECORD:message_4" , "XML?");
	getMessRecordXMLTags[5] = getXML.getValue("REALTAGRECORD:message_5" , "XML?");
   	getMessRecordXMLTags[6] = getXML.getValue("REALTAGRECORD:message_6" , "XML?");
    
	rectX = newRectX;
	rectY = newRectY;
	myFont.loadFont("mono.ttf", 12, false);
	guiText_0.setup();
	guiText_0.setFont(myFont);
	guiText_0.setText(getMessRecordXML[3]);
	rectW = guiText_0.getTextWidth();
	rectH = guiText_0.getTextHeight();
	
	isIndex = true;
    
    playRecordRectColor.set(100, 255, 100, 40);
    recRecordRectColor.set(255, 100, 100, 255);
    playRecordColor.set(100, 255, 100);
    recRecordColor.set(255, 0, 0);

    recIndex_Y = 0;
}

//--------------------------------------------------------------
void guiMessXML::update(float newRectX, float newRectY, string newSetDocDir){
	rectX = newRectX;
	rectY = newRectY;
    if (isUpList == true) {
        recIndex_Y++;
        if (recIndex_Y > maxNumInList) {
            recIndex_Y = 0;
        }
        if (recIndex_Y == maxNumInList){
            getMessRecordXMLTag = "";
        }else {
            getMessRecordXMLTag = getMessRecordXMLTags[recIndex_Y];
        }
        isUpList = false;
        //cout << getMessRecordXMLTag << "       " << recIndex_Y << " isUpList " <<  endl;
    }
    if (isDownList == true) {
        recIndex_Y--;
        if (recIndex_Y < 0) {
            recIndex_Y = maxNumInList;
        }
        if (recIndex_Y == maxNumInList){
            getMessRecordXMLTag = "";
        }else {
            getMessRecordXMLTag = getMessRecordXMLTags[recIndex_Y];
        }
        isDownList = false;
        //cout << getMessRecordXMLTag << "       " << recIndex_Y << " isDownList " << endl;
    }
}

//--------------------------------------------------------------
void guiMessXML::draw(){
	ofFill();
    ofEnableAlphaBlending();
    ofSetColor(playRecordRectColor);
	ofRect(rectX-15, rectY-128, rectW+13, rectH*maxNumInList-3);
    if (recIndex_Y >= maxNumInList) {
        ofSetColor(playRecordRectColor);
    }else {
        ofSetColor(recRecordRectColor);
    }
    ofNoFill();
    ofRect(rectX-15, rectY-128, rectW+13, rectH*maxNumInList-3);
    int rectXW = rectX+rectW+5;
    
    ofFill();
    if (recIndex_Y == 0) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(file, rectXW, rectY-110);
    guiText_0.renderString(getMessRecordXML[0], rectX-10, rectY-110);
    
    if (recIndex_Y == 1) {
        ofSetColor(255,255,255);
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(dateTime, rectXW, rectY-95);
    guiText_0.renderString(getMessRecordXML[1], rectX-10, rectY-95);
    
    if (recIndex_Y == 2) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(session, rectXW, rectY-80);
	guiText_0.renderString(getMessRecordXML[2], rectX-10, rectY-80);
    
    if (recIndex_Y == 3) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(participant, rectXW, rectY-65);
	guiText_0.renderString(getMessRecordXML[3], rectX-10, rectY-65);
    
    if (recIndex_Y == 4) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(age, rectXW, rectY-50);
	guiText_0.renderString(getMessRecordXML[4], rectX-10, rectY-50);
    
    if (recIndex_Y == 5) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(amputation, rectXW, rectY-35);
	guiText_0.renderString(getMessRecordXML[5], rectX-10, rectY-35);
    
    if (recIndex_Y == 6) {
        ofSetColor(recRecordColor);
    }else {
        ofSetColor(playRecordColor);
    }
    guiText_0.renderString(audio, rectXW, rectY-20);
	guiText_0.renderString(getMessRecordXML[6], rectX-10, rectY-20);
   	ofFill();
    ofDisableAlphaBlending();
}
