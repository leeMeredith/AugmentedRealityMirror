//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class recRecordARM{

	public:

		void setup(int setX, int setY, string xmlFile);
        void update(string xmlFile);
		void updateAddress(float setRecordAddress);
        void updateInfo(string setSession, string setParticipant, string setAge, string setAmputation, string setAudio);
        void updateClearTagContents(string newWhoClearTagCon, string newWhichTagCon);
        void setRGBA(int setR, int setG, int setB, int setA);
		void draw(int setX, int setY, int setW, int setH);
		void keyPressed(int key);
        
        bool dBug, isLogging, isLoggingKey;
        string xmlName, xmlFileTest, settingsDirectory, currentXmlFile;
        string whoClearTagCon, whichTagCon;
        bool isRemoveTag;
        bool isRemoveLastRecTag;
        int lastTagNumberRemove;
        float recordAddress, lastEvents;
        int setRecordAddress, setCameraAddress, setRecordAddressTest;
        bool isSetRecord, isSetRecordAddressNew;
        int cameraSize, timesSize, painScoreSize, positionsSize, xAxisSize;
        int recordCamera, recordPainScore;
        string recordTime;
        float recordPosition_X, recordPosition_Y, recordPosition_Z;
        float recordXAxis_X, recordXAxis_Y, recordXAxis_Z;
        float recordYAxis_X, recordYAxis_Y, recordYAxis_Z;
        float recordZAxis_X, recordZAxis_Y, recordZAxis_Z;
        string getDate, getCamera, getSession, getParticipant, getAge, getAmputation, getAudio, getTime, getXAxisString, getYAxisString, getZAxisString, getScaleString, getEndRecString;
        string date;
        string RecordARM;
        string recordSession, recordParticipant, recordAge, recordAmputation, recordAudio;
        vector <int> camera;
        vector <int> painScore;
        vector <string> time;
        vector <float> position_X;
        vector <float> position_Y;
        vector <float> position_Z;
        vector <float> xAxis_X;
        vector <float> xAxis_Y;
        vector <float> xAxis_Z;
        vector <float> yAxis_X;
        vector <float> yAxis_Y;
        vector <float> yAxis_Z;
        vector <float> zAxis_X;
        vector <float> zAxis_Y;
        vector <float> zAxis_Z;
        vector <float> scale;
        vector <float> endRec;
        
        bool isRecord, isSaveAll, isSaveEvents, isStartEvents, isEventsTest, isNothing, isXAxis, isYAxis, isZAxis, isScale, isEndRec;
        bool clearDoc, clearTagCon;
        
        bool isGetAll;
        string guiDateTime, guiSession, guiParticipant, guiAge, guiAmputation, guiAudio;
        string guiCamera, guiPainScore, guiTime;
        string guiPosition_X, guiPosition_Y, guiPosition_Z;
        string guiRecXAxis_X, guiRecXAxis_Y, guiRecXAxis_Z;
        string guiRecYAxis_X, guiRecYAxis_Y, guiRecYAxis_Z;
        string guiRecZAxis_X, guiRecZAxis_Y, guiRecZAxis_Z;
        string guiRecScale, guiRecEndRec;
        float guiRecordVal;
        int guiPainScoreVal, guiCameraVal;
        string guiTimeVal;
        float guiPositionXVal, guiPositionYVal, guiPositionZVal; 
        float guiXAxisXVal, guiXAxisYVal, guiXAxisZVal;
        float guiYAxisXVal, guiYAxisYVal, guiYAxisZVal;
        float guiZAxisXVal, guiZAxisYVal, guiZAxisZVal;
        float guiScaleVal, guiEndRecVal;
        vector <int> allCamera;
        vector <int> allPainScores;
        vector <string> allTimes;
        int getAllPositionsXSize;
        vector <float> allPositions_X;
        vector <float> allPositions_Y;
        vector <float> allPositions_Z;
        vector <float> allXAxis_X;
        vector <float> allXAxis_Y;
        vector <float> allXAxis_Z;
        vector <float> allYAxis_X;
        vector <float> allYAxis_Y;
        vector <float> allYAxis_Z;
        vector <float> allZAxis_X;
        vector <float> allZAxis_Y;
        vector <float> allZAxis_Z;
        vector <float> allScale;
        vector <float> allEndRec;
    
        int setGetRecordAddress, setGetPainScoreAddress, setGetRecordAddressAll, setGetCameraAddress, setGetTimesAddress;
        int setGetPositionsAddress_X, setGetPositionsAddress_Y, setGetPositionsAddress_Z;
        int setGetXAxisXAddress, setGetXAxisYAddress, setGetXAxisZAddress;
        int setGetYAxisXAddress, setGetYAxisYAddress, setGetYAxisZAddress;
        int setGetZAxisXAddress, setGetZAxisYAddress, setGetZAxisZAddress;
        int setGetScaleAddress, setGetEndRecAddress;
        int lastTagNumberEndRec;
        bool isCameraAddressEnd, isTimesAddressEnd, isPainScoresAddressEnd, isPositionsAddressEnd, isXAxisAddressEnd, isYAxisAddressEnd, isZAxisAddressEnd;
        
        int x, y, w, h;
		int r, g, b, a;
    
        ofxXmlSettings settingsRecordARMFile;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;

		int lastRecTagNumber;
};

