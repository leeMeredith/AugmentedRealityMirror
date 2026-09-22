//
//  Created by Lee Meredith.
//

#include "guiMessJson.h"

static bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}

//-------------------------------------------
bool guiMessJson::checkHit(float x, float y){
	if(isInsideRect(x, y, boundingBox)){
		isIn = true;
	}else {
        isIn = false;
    }
    return isIn;
}

//--------------------------------------------------------------
void guiMessJson::setup(float newRectX, float newRectY, string newSetDocDir){
    
    boundingBox.set(rectX-15, rectY-128, rectW+13, rectH*10-3);
	
	setDocDir = newSetDocDir;//message_presets_ARM.json;
	message = "message_presets_ARM.json";
	if( jsonDocument.load(ofToDataPath(setDocDir)) ){
        //jsonDocument.setTo("MESSAGES");
		cout << "message_presets_ARM.json loaded from documents folder!" << endl;
	}else{
        jsonDocument.addChild("MESSAGES");
        jsonDocument.setTo("MESSAGES");
		cout << "unable to load message_presets_ARM.json check data/ folder" << endl;
	}
    
    if(jsonDocument.exists("RECORD")){
        jsonDocument.setTo("RECORD");
        getMessRecordJson[0] = jsonDocument.getValue<string>("message[0]", "JSON?");
        getMessRecordJson[1] = jsonDocument.getValue<string>("message[1]", "JSON?");
        getMessRecordJson[2] = jsonDocument.getValue<string>("message[2]", "JSON?");
        getMessRecordJson[3] = jsonDocument.getValue<string>("message[3]", "JSON?");
        getMessRecordJson[4] = jsonDocument.getValue<string>("message[4]", "JSON?");
        getMessRecordJson[5] = jsonDocument.getValue<string>("message[5]", "JSON?");
        getMessRecordJson[6] = jsonDocument.getValue<string>("message[6]", "JSON?");
    }
    jsonDocument.load(ofToDataPath(setDocDir));
    maxNumInList = 7;
    if(jsonDocument.exists("REALTAGRECORD")){
        jsonDocument.setTo("REALTAGRECORD");
        getMessRecordJsonTags[0] = getMessRecordJsonTag = jsonDocument.getValue<string>("message[0]", "JSON?");
        cout << " getMessRecordJsonTags " << getMessRecordJsonTag << endl;
        getMessRecordJsonTags[1] = jsonDocument.getValue<string>("message[1]", "JSON?");
        getMessRecordJsonTags[2] = jsonDocument.getValue<string>("message[2]", "JSON?");
        getMessRecordJsonTags[3] = jsonDocument.getValue<string>("message[3]", "JSON?");
        getMessRecordJsonTags[4] = jsonDocument.getValue<string>("message[4]", "JSON?");
        getMessRecordJsonTags[5] = jsonDocument.getValue<string>("message[5]", "JSON?");
        getMessRecordJsonTags[6] = jsonDocument.getValue<string>("message[6]", "JSON?");
    }

//    
//    getMessRecordJson[0] = jsonDocument.getValue("RECORD:message_0" , "JSON?");
//	getMessRecordJson[1] = jsonDocument.getValue("RECORD:message_1" , "JSON?");
//	getMessRecordJson[2] = jsonDocument.getValue("RECORD:message_2" , "JSON?");
//	getMessRecordJson[3] = jsonDocument.getValue("RECORD:message_3" , "JSON?");
//	getMessRecordJson[4] = jsonDocument.getValue("RECORD:message_4" , "JSON?");
//	getMessRecordJson[5] = jsonDocument.getValue("RECORD:message_5" , "JSON?");
//   	getMessRecordJson[6] = jsonDocument.getValue("RECORD:message_6" , "JSON?");
//    
//    getMessRecordJsonTags[0] = getMessRecordJsonTag = jsonDocument.getValue("REALTAGRECORD:message_0" , "JSON?");
//	getMessRecordJsonTags[1] = jsonDocument.getValue("REALTAGRECORD:message_1" , "JSON?");
//	getMessRecordJsonTags[2] = jsonDocument.getValue("REALTAGRECORD:message_2" , "JSON?");
//	getMessRecordJsonTags[3] = jsonDocument.getValue("REALTAGRECORD:message_3" , "JSON?");
//	getMessRecordJsonTags[4] = jsonDocument.getValue("REALTAGRECORD:message_4" , "JSON?");
//	getMessRecordJsonTags[5] = jsonDocument.getValue("REALTAGRECORD:message_5" , "JSON?");
//   	getMessRecordJsonTags[6] = jsonDocument.getValue("REALTAGRECORD:message_6" , "JSON?");
    
	rectX = newRectX;
	rectY = newRectY;
	myFont.loadFont("mono.ttf", 15, false);
	guiText_0.setup();
	guiText_0.setFont(myFont);
	guiText_0.setText(getMessRecordJson[3]);
	rectW = guiText_0.getTextWidth();
	rectH = guiText_0.getTextHeight();
	
    //cursor-----------------------_
    cursor_X = rectX-10;
    cursor_Y = rectY-110;
    cursor_W = 1;
	cursor_H = guiText_0.getTextHeight();
    isSetCursorAtSizeOne_X = isSetCursorAtSizeZero_X = isSetCursorAtSize_X = false;
    cursor.set(cursor_X, cursor_Y, cursor_W, cursor_H);
    
    cursorCounter = 0;
    newCursor_X = 0;
    cursor_Y = rectY-110;
    stringSize = 0;
    
    cursorCounter = ofGetSeconds();
    testCursorCounter = true;
    //-----------cursor------------_
	
	isIndex = true;
    isSel = false;
    isSetAllEnter = false;
    isFileEnter = isDateTimeEnter = isSessionEnter = isParticipantEnter = isAgeEnter = isAmputationEnter = isAudioEnter = false;
    
    playRecordRectColor.set(100, 255, 100, 40);
    recRecordRectColor.set(255, 100, 100, 255);
    playRecordColor.set(100, 255, 100);
    recRecordColor.set(255, 0, 255, 255);
    inRecordRectColor.set(255, 255, 255);
    selRecordRectColor.set(255, 255, 255);
    enterColor.set(255, 0, 0, 255);

    recIndex_Y = 0;
}

//--------------------------------------------------------------
void guiMessJson::update(float newRectX, float newRectY, string newSetDocDir){
	rectX = newRectX;
	rectY = newRectY;
    boundingBox.set(rectX-15, rectY-128, rectW+13, rectH*10-3);
    
    //cursor-----------------------_
    if (recIndex_Y != maxNumInList) {
        if (cursorCounter != ofGetSeconds()) {
            testCursorCounter = !testCursorCounter;
            cursorCounter = ofGetSeconds();
        }
    }
    //-----------cursor------------_
    
    int rectXW = rectX+rectW+5;
    
    if (recIndex_Y == maxNumInList) {
        isSetAllEnter = true;
        newCursor_X = rectXW;
        cursor_Y = rectY-110;
        stringSize = file.size();
    }
    
    if (recIndex_Y == 0) {
        newCursor_X = rectXW;
        cursor_Y = rectY-110;
        stringSize = file.size();
    }
    
    if (recIndex_Y == 1) {
        newCursor_X = rectXW;
        cursor_Y = rectY-92;
        stringSize = dateTime.size();
    }
    
    if (recIndex_Y == 2) {
        newCursor_X = rectXW;
        cursor_Y = rectY-74;
        stringSize = session.size();
    }
    
    if (recIndex_Y == 3) {
        newCursor_X = rectXW;
        cursor_Y = rectY-56;
        stringSize = participant.size();
    }
    
    if (recIndex_Y == 4) {
        newCursor_X = rectXW;
        cursor_Y = rectY-38;
        stringSize = age.size();
    }
    
    if (recIndex_Y == 5) {
        newCursor_X = rectXW;
        cursor_Y = rectY-20;
        stringSize = amputation.size();
    }
    
    if (recIndex_Y == 6) {
        newCursor_X = rectXW;
        cursor_Y = rectY-2;
        stringSize = audio.size();
    }
   
    
    if (isUpList == true) {
        recIndex_Y++;
        testCursorCounter = true;

        int rectXW = rectX+rectW+5;
        if (recIndex_Y == maxNumInList) {
            isSetAllEnter = true;
            newCursor_X = rectXW;
            cursor_Y = rectY-110;
            stringSize = file.size();
        }
        
        if (recIndex_Y == 0) {
            newCursor_X = rectXW;
            cursor_Y = rectY-110;
            stringSize = file.size();
        }
        
        if (recIndex_Y == 1) {
            newCursor_X = rectXW;
            cursor_Y = rectY-92;
            stringSize = dateTime.size();
        }
        
        if (recIndex_Y == 2) {
            newCursor_X = rectXW;
            cursor_Y = rectY-74;
            stringSize = session.size();
        }
        
        if (recIndex_Y == 3) {
            newCursor_X = rectXW;
            cursor_Y = rectY-56;
            stringSize = participant.size();
        }
        
        if (recIndex_Y == 4) {
            newCursor_X = rectXW;
            cursor_Y = rectY-38;
            stringSize = age.size();
        }
        
        if (recIndex_Y == 5) {
            newCursor_X = rectXW;
            cursor_Y = rectY-20;
            stringSize = amputation.size();
        }
        
        if (recIndex_Y == 6) {
            newCursor_X = rectXW;
            cursor_Y = rectY-2;
            stringSize = audio.size();
        }

        if (recIndex_Y > maxNumInList) {
            recIndex_Y = 0;
        }
        if (recIndex_Y == maxNumInList){
            getMessRecordJsonTag = "";
        }else {
            getMessRecordJsonTag = getMessRecordJsonTags[recIndex_Y];
        }
        cursor_X = newCursor_X + ((float)guiText_0.fontSize-1) * stringSize;
        cursorRightLeft = 0;
        stringIndexRightLeft = stringSize-1; 
        isUpList = false;
        //cout << getMessRecordJsonTag << "       " << recIndex_Y << " isUpList " <<  endl;
    }
    if (isDownList == true) {
        recIndex_Y--;
        testCursorCounter = true;
        
        int rectXW = rectX+rectW+5;
        if (recIndex_Y == maxNumInList) {
            isSetAllEnter = true;
            newCursor_X = rectXW;
            cursor_Y = rectY-110;
            stringSize = file.size();
        }
        
        if (recIndex_Y == 0) {
            newCursor_X = rectXW;
            cursor_Y = rectY-110;
            stringSize = file.size();
        }
        
        if (recIndex_Y == 1) {
            newCursor_X = rectXW;
            cursor_Y = rectY-92;
            stringSize = dateTime.size();
        }
        
        if (recIndex_Y == 2) {
            newCursor_X = rectXW;
            cursor_Y = rectY-74;
            stringSize = session.size();
        }
        
        if (recIndex_Y == 3) {
            newCursor_X = rectXW;
            cursor_Y = rectY-56;
            stringSize = participant.size();
        }
        
        if (recIndex_Y == 4) {
            newCursor_X = rectXW;
            cursor_Y = rectY-38;
            stringSize = age.size();
        }
        
        if (recIndex_Y == 5) {
            newCursor_X = rectXW;
            cursor_Y = rectY-20;
            stringSize = amputation.size();
        }
        
        if (recIndex_Y == 6) {
            newCursor_X = rectXW;
            cursor_Y = rectY-2;
            stringSize = audio.size();
        }
        
        if (recIndex_Y < 0) {
            recIndex_Y = maxNumInList;
        }
        if (recIndex_Y == maxNumInList){
            getMessRecordJsonTag = "";
        }else {
            getMessRecordJsonTag = getMessRecordJsonTags[recIndex_Y];
        }
        cursor_X = newCursor_X + ((float)guiText_0.fontSize-1) * stringSize;
        cursorRightLeft = 0;
        stringIndexRightLeft = stringSize-1; 
        isDownList = false;
        //cout << getMessRecordJsonTag << "       " << recIndex_Y << " isDownList " << endl;
    }
    
    if (isLeft == true) {
        if (recIndex_Y != 0 && recIndex_Y != 1 && recIndex_Y != 6){
            cursorRightLeft++;
            stringIndexRightLeft--;
        }
        testCursorCounter = true;
        
        if (cursorRightLeft >= stringSize) {
            cursorRightLeft = stringSize-1;
            //stringIndexRightLeft = stringSize-1;
        }
        if (stringIndexRightLeft <= 0) {
            stringIndexRightLeft = 0;
        }
        
        isLeft = false;
        cout << "cursor " << cursorRightLeft << " stringIndex " << stringIndexRightLeft << endl;
    }
    if (isRight == true) {
        if (recIndex_Y != 0 && recIndex_Y != 1 && recIndex_Y != 6){
            cursorRightLeft--;
            stringIndexRightLeft++;
        }
        testCursorCounter = true;
        ///*
        if (cursorRightLeft < 0) {
            cursorRightLeft = stringSize-1;
            //stringIndexRightLeft = 0;
        }
        if (stringIndexRightLeft >= stringSize) {
            stringIndexRightLeft = 0;
        }
        //*/
        isRight = false;
        cout << "cursor " << cursorRightLeft << " stringIndex " << stringIndexRightLeft << endl;
    }
    
    if (cursorRightLeft >= stringSize) {
        cursorRightLeft = stringSize-1;
        //stringIndexRightLeft = stringSize-1;
    }
    if (stringIndexRightLeft <= 0) {
        stringIndexRightLeft = 0;
    }
    
    if (cursorRightLeft < 0) {
        cursorRightLeft = stringSize-1;
        //stringIndexRightLeft = 0;
    }
    if (stringIndexRightLeft >= stringSize) {
        stringIndexRightLeft = 0;
    }
    
    if (isSetAllEnter == true) {
        isFileEnter = isDateTimeEnter = isSessionEnter = isParticipantEnter = isAgeEnter = isAmputationEnter = isAudioEnter = false;
        isSetAllEnter = false;
    }
}

//--------------------------------------------------------------
void guiMessJson::draw(){
	ofFill();
    ofEnableAlphaBlending();
    ofSetColor(playRecordRectColor);
    boundingBox.set(rectX-15, rectY-128, rectW+13, rectH*10-3);
	ofRect(rectX-15, rectY-128, rectW+15, rectH*maxNumInList-5);
    if (recIndex_Y >= maxNumInList) {
        ofSetColor(playRecordRectColor);
    }else {
        ofSetColor(recRecordRectColor);
    }
    if (isIn == true) {
        ofSetColor(inRecordRectColor);
    }
    if (isSel == true) {
        ofSetColor(selRecordRectColor);
    }
    ofNoFill();
    ofRect(rectX-15, rectY-128, rectW+15, rectH*maxNumInList-5);
    int rectXW = rectX+rectW+5;
    
    ofFill();
    if (recIndex_Y == 0) {
        if (isFileEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(enterColor);
        }
    }else {
        if (isFileEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(file, rectXW, rectY-110);
    guiText_0.renderString(getMessRecordJson[0], rectX-10, rectY-110);
    
    if (recIndex_Y == 1) {
        if (isDateTimeEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(recRecordColor);
        }
    }else {
        if (isDateTimeEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(dateTime, rectXW, rectY-92);
    guiText_0.renderString(getMessRecordJson[1], rectX-10, rectY-92);
    
    if (recIndex_Y == 2) {
        if (isSessionEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(recRecordColor);
        }
    }else {
        if (isSessionEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(session, rectXW, rectY-74);
	guiText_0.renderString(getMessRecordJson[2], rectX-10, rectY-74);
    
    if (recIndex_Y == 3) {
        if (isParticipantEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(recRecordColor);
        }
    }else {
        if (isParticipantEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(participant, rectXW, rectY-56);
	guiText_0.renderString(getMessRecordJson[3], rectX-10, rectY-56);
    
    if (recIndex_Y == 4) {
        if (isAgeEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(recRecordColor);
        }
    }else {
        if (isAgeEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(age, rectXW, rectY-38);
	guiText_0.renderString(getMessRecordJson[4], rectX-10, rectY-38);
    
    if (recIndex_Y == 5) {
        if (isAmputationEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(recRecordColor);
        }
    }else {
        if (isAmputationEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(amputation, rectXW, rectY-20);
	guiText_0.renderString(getMessRecordJson[5], rectX-10, rectY-20);
    
    if (recIndex_Y == 6) {
        if (isAudioEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(enterColor);
        }
    }else {
        if (isAudioEnter == true) {
            ofSetColor(enterColor);
        }else {
            ofSetColor(playRecordColor);
        }
    }
    guiText_0.renderString(audio, rectXW, rectY-2);
	guiText_0.renderString(getMessRecordJson[6], rectX-10, rectY-2);
    
    if (recIndex_Y != maxNumInList) {
        if (testCursorCounter) {
            ofSetColor(255,255,255,255);
        }else {
            ofSetColor(255,255,255,100);
        }
        if (cursorRightLeft >= 0){
            if (stringSize >= 0) {
                
                if (cursorRightLeft <= stringSize-1) {
                    cursor_X = (newCursor_X + ((float)guiText_0.fontSize-1) * stringSize-(((float)guiText_0.fontSize-1)*cursorRightLeft));
                }
            }
            isSetCursorAtSize_X = true;
            isSetCursorAtSizeZero_X = true;
        }
        ofRect(cursor_X, cursor_Y-cursor_H, cursor_W, cursor_H);
    }
    
   	ofFill();
    ofDisableAlphaBlending();
}
