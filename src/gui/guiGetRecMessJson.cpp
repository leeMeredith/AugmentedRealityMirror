//
//  Created by Lee Meredith.
//

#include "guiGetRecMessJson.h"


//--------------------------------------------------------------
void guiGetRecMessJson::setup(float newRectX, float newRectY, string newSetDocDir){
	
	setDocDir = newSetDocDir;//message_presets_ARM.json;
	message = "record/message/message_presets_ARM.json";
	if( jsonDocument.load(ofToDataPath(setDocDir)) ){
        jsonDocument.setTo("MESSAGES");
		cout << "message_presets_ARM.json loaded from documents folder!" << endl;
	}else{
        jsonDocument.addChild("MESSAGES");
        jsonDocument.setTo("MESSAGES");
		cout << "unable to load message_presets_ARM.json check data/ folder" << endl;
	}
    
    if(jsonDocument.exists("GETRECORD")){
        jsonDocument.setTo("GETRECORD[0]");
        getMessGetRecordJson[0] = jsonDocument.getValue<string>("message[0]", "JSON?");
        getMessGetRecordJson[1] = jsonDocument.getValue<string>("message[1]", "JSON?");
        getMessGetRecordJson[2] = jsonDocument.getValue<string>("message[2]", "JSON?");
        getMessGetRecordJson[3] = jsonDocument.getValue<string>("message[3]", "JSON?");
        getMessGetRecordJson[4] = jsonDocument.getValue<string>("message[4]", "JSON?");
        getMessGetRecordJson[5] = jsonDocument.getValue<string>("message[5]", "JSON?");
        getMessGetRecordJson[6] = jsonDocument.getValue<string>("message[6]", "JSON?");
        getMessGetRecordJson[7] = jsonDocument.getValue<string>("message[7]", "JSON?");
    }
    
	rectX = newRectX;
	rectY = newRectY;
	myFont.loadFont("mono.ttf", 12, false);
	guiText_0.setup();
	guiText_0.setFont(myFont);
	guiText_0.setText(getMessGetRecordJson[1]);
	rectW = guiText_0.getTextWidth();
	rectH = guiText_0.getTextHeight();
	
	isIndex = true;
    
    //recordFloat-----------------------_
    playRecordFloatRectColor.set(100, 255, 100, 40);
    recRecordFloatRectColor.set(255, 100, 100, 40);
    playRecordFloatColor.set(100, 255, 100);
    recRecordFloatColor.set(255, 100, 100);
    
    playRecordRectColor.set(100, 255, 100, 40);
    recRecordRectColor.set(255, 100, 100, 40);
    playRecordColor.set(100, 255, 100);
    recRecordColor.set(255, 100, 100);
    timeVal = "here I am";
    int rectXW = rectX+rectW+15;
    camera.setup(rectXW, rectY-80, cameraVal);
    painScore.setup(rectXW, rectY-65, painScoreVal);

    position_X.setup(rectXW, rectY-35, positionVal_X);
    position_Y.setup(rectXW*2, rectY-35, positionVal_Y);
    position_Z.setup(rectXW*3, rectY-35, positionVal_Z);
    
    xAxis_X.setup(rectXW, rectY-20, xAxisXVal);
    xAxis_Y.setup(rectXW*2, rectY-20, xAxisYVal);
    xAxis_Z.setup(rectXW*3, rectY-20, xAxisZVal);
    
    yAxis_X.setup(rectXW, rectY-5, yAxisXVal);
    yAxis_Y.setup(rectXW*2, rectY-5, yAxisYVal);
    yAxis_Z.setup(rectXW*3, rectY-5, yAxisZVal);
    
    zAxis_X.setup(rectXW, rectY+10, zAxisXVal);
    zAxis_Y.setup(rectXW*2, rectY+10, zAxisYVal);
    zAxis_Z.setup(rectXW*3, rectY+10, zAxisZVal);
    
    endRec.setup(rectXW, rectY+25, endRecVal);
    //------------recordFloat-----------_
    
    //recordInt-----------------------_
    int rectIntXW = rectX+rectW+15;
    cameraSize.setup(rectIntXW, rectY-80, cameraValSize);
    painScoreSize.setup(rectIntXW, rectY-65, painScoreValSize);
    timeSize.setup(rectIntXW, rectY-50, timeValSize);
    positionSize.setup(rectIntXW, rectY-35, positionValSize);
    xAxisSize.setup(rectIntXW, rectY-20, xAxisValSize);
    yAxisSize.setup(rectIntXW, rectY-5, yAxisValSize);
    zAxisSize.setup(rectIntXW, rectY+10, zAxisValSize);
    endRecSize.setup(rectIntXW, rectY+25, endRecValSize);
    //------------recordInt-----------_
}

//--------------------------------------------------------------
void guiGetRecMessJson::update(float newRectX, float newRectY, string newSetDocDir){
	rectX = newRectX;
	rectY = newRectY;
	/*
	//getIndexJson[0] = 1;
	if (isIndex == true) {
		for (int i = 0; i < 6; i++) {
			char tempGetIndexJsonString[255];
			sprintf(tempGetIndexJsonString,"%i", (int)getIndexJson[i]);
			newGetIndexJsonString[i] = tempGetIndexJsonString;
		}
		//isIndex = false;
	}
    */
    
    if(jsonDocument.exists("GETRECORD")){
        jsonDocument.setTo("GETRECORD[0]");
            getMessGetRecordJson[0] = jsonDocument.getValue<string>("message[0]", "JSON?");
            getMessGetRecordJson[1] = jsonDocument.getValue<string>("message[1]", "JSON?");
            getMessGetRecordJson[2] = jsonDocument.getValue<string>("message[2]", "JSON?");
            getMessGetRecordJson[3] = jsonDocument.getValue<string>("message[3]", "JSON?");
            getMessGetRecordJson[4] = jsonDocument.getValue<string>("message[4]", "JSON?");
            getMessGetRecordJson[5] = jsonDocument.getValue<string>("message[5]", "JSON?");
            getMessGetRecordJson[6] = jsonDocument.getValue<string>("message[6]", "JSON?");
            getMessGetRecordJson[7] = jsonDocument.getValue<string>("message[7]", "JSON?");
    }
           
    
    //recordInt-----------------------_
    int rectIntXW = rectX+rectW+10;
    cameraSize.update(rectIntXW, rectY-80, cameraValSize);
    painScoreSize.update(rectIntXW, rectY-65, painScoreValSize);
    timeSize.update(rectIntXW, rectY-50, timeValSize);
    positionSize.update(rectIntXW, rectY-35, positionValSize);
    xAxisSize.update(rectIntXW, rectY-20, xAxisValSize);
    yAxisSize.update(rectIntXW, rectY-5, yAxisValSize);
    zAxisSize.update(rectIntXW, rectY+10, zAxisValSize);
    endRecSize.update(rectIntXW, rectY+25, endRecValSize);
    
    cameraSize.rectTextColor = playRecordFloatRectColor;
    painScoreSize.rectTextColor = playRecordFloatRectColor;
    timeSize.rectTextColor = playRecordFloatRectColor;
    positionSize.rectTextColor = playRecordFloatRectColor;
    xAxisSize.rectTextColor = playRecordFloatRectColor;
    yAxisSize.rectTextColor = playRecordFloatRectColor;
    zAxisSize.rectTextColor = playRecordFloatRectColor;
    endRecSize.rectTextColor = playRecordFloatRectColor;
    
    cameraSize.textColor = cameraFloatColor;
    painScoreSize.textColor = painScoreIntColor;
    timeSize.textColor = timeFloatColor;
    positionSize.textColor = positionFloatColor;
    xAxisSize.textColor = xAxisFloatColor;
    yAxisSize.textColor = yAxisFloatColor;
    zAxisSize.textColor = zAxisFloatColor;
    endRecSize.textColor = endRecFloatColor;
    //------------recordInt-----------_    
    
    //recordFloat-----------------------_
    int rectXW = rectX+rectW+60;
    camera.update(rectXW, rectY-80, cameraVal);
    painScore.update(rectXW, rectY-65, painScoreVal);
    
    position_X.update(rectXW, rectY-35, positionVal_X);
    position_Y.update(rectXW*2, rectY-35, positionVal_Y);
    position_Z.update(rectXW*3, rectY-35, positionVal_Z);
    
    xAxis_X.update(rectXW, rectY-20, xAxisXVal);
    xAxis_Y.update(rectXW*2, rectY-20, xAxisYVal);
    xAxis_Z.update(rectXW*3, rectY-20, xAxisZVal);
    
    yAxis_X.update(rectXW, rectY-5, yAxisXVal);
    yAxis_Y.update(rectXW*2, rectY-5, yAxisYVal);
    yAxis_Z.update(rectXW*3, rectY-5, yAxisZVal);
    
    zAxis_X.update(rectXW, rectY+10, zAxisXVal);
    zAxis_Y.update(rectXW*2, rectY+10, zAxisYVal);
    zAxis_Z.update(rectXW*3, rectY+10, zAxisZVal);
    
    endRec.update(rectXW, rectY+25, endRecVal);
    
    camera.rectTextColor = playRecordFloatRectColor;
    painScore.rectTextColor = playRecordFloatRectColor;
    position_X.rectTextColor = position_Y.rectTextColor = position_Z.rectTextColor = playRecordFloatRectColor;
    xAxis_X.rectTextColor = xAxis_Y.rectTextColor = xAxis_Z.rectTextColor = playRecordFloatRectColor;
    yAxis_X.rectTextColor = yAxis_Y.rectTextColor = yAxis_Z.rectTextColor = playRecordFloatRectColor;
    zAxis_X.rectTextColor = zAxis_Y.rectTextColor = zAxis_Y.rectTextColor = playRecordFloatRectColor;
    endRec.rectTextColor = playRecordFloatRectColor;
    
    camera.textColor = recRecordFloatColor;
    painScore.textColor = painScoreIntColor;
    
    position_X.textColor = position_Y.textColor = position_Z.textColor = positionFloatColor;
    xAxis_X.textColor = xAxis_Y.textColor = xAxis_Z.textColor = xAxisFloatColor;
    yAxis_X.textColor = yAxis_Y.textColor = yAxis_Z.textColor = yAxisFloatColor;
    zAxis_X.textColor = zAxis_Y.textColor = zAxis_Z.textColor = zAxisFloatColor;
    endRec.textColor = endRecFloatColor;
    //------------recordFloat-----------_

}

//--------------------------------------------------------------
void guiGetRecMessJson::draw(){
	ofFill();
    ofEnableAlphaBlending();
	ofSetColor(playRecordRectColor);
	ofRect(rectX-15, rectY-98, rectW+61, rectH*10);
    
    ofNoFill();
	ofSetColor(playRecordColor);
	ofRect(rectX-15, rectY-98, rectW+61, rectH*10);
	
    ofFill();
    int rectXW = rectX+rectW+50;
    ofSetColor(playRecordColor);//ofSetColor(playRecordRectColor);
	guiText_0.renderString(getMessGetRecordJson[0], rectX-10, rectY-80);
	guiText_0.renderString(getMessGetRecordJson[1], rectX-10, rectY-65);
	guiText_0.renderString(getMessGetRecordJson[2], rectX-10, rectY-50);
    guiText_0.renderString(timeVal, rectXW, rectY-50);
	guiText_0.renderString(getMessGetRecordJson[3], rectX-10, rectY-35);
	guiText_0.renderString(getMessGetRecordJson[4], rectX-10, rectY-20);
    guiText_0.renderString(getMessGetRecordJson[5], rectX-10, rectY-5);
    guiText_0.renderString(getMessGetRecordJson[6], rectX-10, rectY+10);
    guiText_0.renderString(getMessGetRecordJson[7], rectX-10, rectY+25);
    
    //recordInt-----------------------_
    cameraSize.draw();
    painScoreSize.draw();
    timeSize.draw();
    positionSize.draw();
    xAxisSize.draw();
    yAxisSize.draw();
    zAxisSize.draw();
    endRecSize.draw();
    //------------recordInt-----------_
    
    //recordFloat-----------------------_
    camera.draw();
    painScore.draw();
    
    position_X.draw();
    position_Y.draw();
    position_Z.draw();

    xAxis_X.draw();
    xAxis_Y.draw();
    xAxis_Z.draw();
    
    yAxis_X.draw();
    yAxis_Y.draw();
    yAxis_Z.draw();
    
    zAxis_X.draw();
    zAxis_Y.draw();
    zAxis_Z.draw();
    
    endRec.draw();
    //------------recordFloat-----------_
    ofDisableAlphaBlending();
}
