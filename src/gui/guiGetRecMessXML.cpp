//
//  Created by Lee Meredith.
//

#include "guiGetRecMessXML.h"


//--------------------------------------------------------------
void guiGetRecMessXML::setup(float newRectX, float newRectY, string newSetDocDir){
	
	setDocDir = newSetDocDir;//message_presets.xml;
	message = "record/message/message_presets_ARM.xml";
	if( getXML.load(setDocDir) ){
        getXML.setTo("MESSAGES");
		cout << "message_presets.xml loaded from documents folder!" << endl;
	}else{
        getXML.addChild("MESSAGES");
        getXML.setTo("MESSAGES");
		cout << "unable to load message_presets_ARM.xml check data/ folder" << endl;
	}
    
    if(getXML.exists("GETRECORD")){
        getXML.setTo("GETRECORD[0]");
        getMessGetRecordXML[0] = getXML.getValue<string>("message[0]", "XML?");
        getMessGetRecordXML[1] = getXML.getValue<string>("message[1]", "XML?");
        getMessGetRecordXML[2] = getXML.getValue<string>("message[2]", "XML?");
        getMessGetRecordXML[3] = getXML.getValue<string>("message[3]", "XML?");
        getMessGetRecordXML[4] = getXML.getValue<string>("message[4]", "XML?");
        getMessGetRecordXML[5] = getXML.getValue<string>("message[5]", "XML?");
        getMessGetRecordXML[6] = getXML.getValue<string>("message[6]", "XML?");
        getMessGetRecordXML[7] = getXML.getValue<string>("message[7]", "XML?");
    }
    
	rectX = newRectX;
	rectY = newRectY;
	myFont.loadFont("mono.ttf", 12, false);
	guiText_0.setup();
	guiText_0.setFont(myFont);
	guiText_0.setText(getMessGetRecordXML[1]);
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
void guiGetRecMessXML::update(float newRectX, float newRectY, string newSetDocDir){
	rectX = newRectX;
	rectY = newRectY;
	/*
	//getIndexXML[0] = 1;
	if (isIndex == true) {
		for (int i = 0; i < 6; i++) {
			char tempGetIndexXMLString[255];
			sprintf(tempGetIndexXMLString,"%i", (int)getIndexXML[i]);
			newGetIndexXMLString[i] = tempGetIndexXMLString;
		}
		//isIndex = false;
	}
    */
    
    if(getXML.exists("GETRECORD")){
        getXML.setTo("GETRECORD[0]");
            getMessGetRecordXML[0] = getXML.getValue<string>("message[0]", "XML?");
            getMessGetRecordXML[1] = getXML.getValue<string>("message[1]", "XML?");
            getMessGetRecordXML[2] = getXML.getValue<string>("message[2]", "XML?");
            getMessGetRecordXML[3] = getXML.getValue<string>("message[3]", "XML?");
            getMessGetRecordXML[4] = getXML.getValue<string>("message[4]", "XML?");
            getMessGetRecordXML[5] = getXML.getValue<string>("message[5]", "XML?");
            getMessGetRecordXML[6] = getXML.getValue<string>("message[6]", "XML?");
            getMessGetRecordXML[7] = getXML.getValue<string>("message[7]", "XML?");
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
void guiGetRecMessXML::draw(){
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
	guiText_0.renderString(getMessGetRecordXML[0], rectX-10, rectY-80);
	guiText_0.renderString(getMessGetRecordXML[1], rectX-10, rectY-65);
	guiText_0.renderString(getMessGetRecordXML[2], rectX-10, rectY-50);
    guiText_0.renderString(timeVal, rectXW, rectY-50);
	guiText_0.renderString(getMessGetRecordXML[3], rectX-10, rectY-35);
	guiText_0.renderString(getMessGetRecordXML[4], rectX-10, rectY-20);
    guiText_0.renderString(getMessGetRecordXML[5], rectX-10, rectY-5);
    guiText_0.renderString(getMessGetRecordXML[6], rectX-10, rectY+10);
    guiText_0.renderString(getMessGetRecordXML[7], rectX-10, rectY+25);
    
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
