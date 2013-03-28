//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiText.h"
#include "recordFloat.h"
#include "recordInt.h"

class guiGetRecMessXML{
	
public:
	void setup(float newRectX, float newRectY, string newSetDocDir);
	void update(float newRectX, float newRectY, string newSetDocDir);
	void draw();
	
	//guiText-----_
	guiText guiText_0;
	ofTrueTypeFont myFont;
	//---guiText--_
	
	float rectX, rectY;
	float rectW, rectH;
	
	int messIndex;
	
	//xml--------------_
	ofxXmlSettings getXML;
	bool isIndex;
	string message, setDocDir;
	//-------xml-------_
    
    //recordInt-----------------------_
    recordInt cameraSize;
    recordInt painScoreSize;
    recordInt timeSize;
    recordInt positionSize;
    recordInt xAxisSize;
    recordInt yAxisSize;
    recordInt zAxisSize;
    recordInt endRecSize;
    int cameraValSize, painScoreValSize, timeValSize, positionValSize, xAxisValSize, yAxisValSize, zAxisValSize, endRecValSize;
    //------------recordInt-----------_
    
    //recordFloat-----------------------_
    ofColor playRecordFloatColor, recRecordFloatColor;
    ofColor playRecordFloatRectColor, recRecordFloatRectColor;
    ofColor playRecordColor, recRecordColor;
    ofColor playRecordRectColor, recRecordRectColor;
    string getMessGetRecordXML[8], timeVal;
    float positionVal_X, positionVal_Y, positionVal_Z;
    
    float xAxisXVal, xAxisYVal, xAxisZVal;
    float yAxisXVal, yAxisYVal, yAxisZVal;
    float zAxisXVal, zAxisYVal, zAxisZVal;
    float endRecVal;
    int cameraVal, painScoreVal;
    recordInt camera;
    recordInt painScore;
    recordFloat position_X, position_Y, position_Z;
    recordFloat xAxis_X, xAxis_Y, xAxis_Z;
    recordFloat yAxis_X, yAxis_Y, yAxis_Z;
    recordFloat zAxis_X, zAxis_Y, zAxis_Z;
    recordFloat endRec;
    ofColor cameraFloatColor;
    ofColor painScoreIntColor;
    ofColor timeFloatColor;
    ofColor positionFloatColor;
    ofColor xAxisFloatColor;
    ofColor yAxisFloatColor;
    ofColor zAxisFloatColor;
    ofColor endRecFloatColor;
    //---------recordFloat--------------_
    
};