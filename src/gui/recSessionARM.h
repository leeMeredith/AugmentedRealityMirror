//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"

class recSessionARM{

	public:

		void setup(int setX, int setY, string xmlFile);
        void update(string xmlFile);
		void updateAddress(float setRecordAddress);
        void updateInfo(string setSession, string setParticipant, string setAge, string setAmputation, string setAudio);
        void updateClearTagContents(string newWhoClearTagCon, string newWhichTagCon);
        void setRGBA(int setR, int setG, int setB, int setA);
		void draw(int setX, int setY, int setW, int setH);
		void keyPressed(int key);
        
        bool dBug, isLogging, isLoggingKey, isNewXmlFile;;
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
    
        vector <int> sessionCamera;
        vector <int> sessionPainScore;
        vector <string> sessionTime;
        vector <float> sessionPosition_X;
        vector <float> sessionPosition_Y;
        vector <float> sessionPosition_Z;
        vector <float> sessionXAxis_X;
        vector <float> sessionXAxis_Y;
        vector <float> sessionXAxis_Z;
        vector <float> sessionYAxis_X;
        vector <float> sessionYAxis_Y;
        vector <float> sessionYAxis_Z;
        vector <float> sessionZAxis_X;
        vector <float> sessionZAxis_Y;
        vector <float> sessionZAxis_Z;
        vector <float> sessionScale;
        vector <float> sessionEndRec;
        
        bool isRecord, isSaveAll, isSaveEvents, isStartEvents, isEventsTest, isNothing, isXAxis, isYAxis, isZAxis, isScale, isEndRec;
        bool clearDoc, clearTagCon;
        bool wasSaved, isClearSave;
        
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
    
        ofXml settingsRecordARMFile;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;

		int lastRecTagNumber;
    
};

