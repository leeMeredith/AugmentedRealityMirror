//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiText.h"
#include "recordFloat.h"
#include "recordInt.h"

class guiMessXML{
	
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
	int maxNumInList;
    
	//xml--------------_
	ofxXmlSettings getXML;
	bool isIndex;
	string message, setDocDir;
	//-------xml-------_
    
    int recIndex_Y, recIndexTest;
    bool isUpList, isDownList;
    
    ofColor playRecordColor, recRecordColor;
    ofColor playRecordRectColor, recRecordRectColor;
    
    string getMessRecordXMLTag;
    string getMessRecordXML[10];
    string getMessRecordXMLTags[10];
    string file, dateTime, session, participant, age, amputation, audio;
    
    //recordFloat-----------------------_
    //recordFloat recordFloat_0;
    //---------recordFloat--------------_
    
};