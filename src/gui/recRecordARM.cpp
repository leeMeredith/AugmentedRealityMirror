//
//  Created by Lee Meredith.
//

#include "recRecordARM.h"

//--------------------------------------------------------------
void recRecordARM::setup(int setX, int setY, string xmlFile){
    
    recordAddress = 0.1;
    dBug = false;
    
    x = setX;
    y = setY;
    
    xmlFileTest = xmlFile;
    currentXmlFile = "record/address/";
    currentXmlFile += xmlFile;
    settingsDirectory = currentXmlFile;
    
    int posLastSlash = settingsDirectory.rfind("/");
    if(posLastSlash > 0)settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
    else settingsDirectory = "";
    //currentXmlFile = settingsDirectory + currentXmlFile;
    int posLastDot = currentXmlFile.rfind(".xml");
    if (posLastDot < 1) {
        currentXmlFile += ".xml";
    }
    //currentXmlFile += "/";
    
    //the string is printed at the top of the app
	//to give the user some feedback
	message = "loading ";
    message += currentXmlFile;
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( settingsRecordARMFile.loadFile(currentXmlFile) ){
        message = currentXmlFile;
		message += " loaded!";
        cout << message << endl;
	}else{
		message = "unable to load ";
        message += currentXmlFile;
        cout << message << endl;
	}

    isRemoveTag = false;
    lastTagNumberRemove = 0;
    
    date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
    isNothing = false;
    isEventsTest = true;
    clearDoc = clearTagCon = false;
    whoClearTagCon = "SESSION";
    whichTagCon = "tesuji";
    recordAddress = 0.1;
    recordSession = "0";
    recordParticipant = "who";
    recordAge = "120";
    recordAmputation = "BK";
    recordAudio = "heyYou.wav";
	    
	lastRecTagNumber = 0;    
	
    //-------
	//this is a more advanced use of ofXMLSettings
	//we are going to be reading multiple tags with the same name
    
	//lets see how many <RECORD> </RECORD> tags there are in the xml file
	int numAboutTags = settingsRecordARMFile.getNumTags("about");
    
	//if there is at least one <RECORD> tag we can read the list of points
	//and then try and draw it as a line on the screen
	if(numAboutTags == 0){
        //client about
        settingsRecordARMFile.addTag("about");
        settingsRecordARMFile.pushTag("about");
        settingsRecordARMFile.addValue("file", currentXmlFile);
        date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        settingsRecordARMFile.addValue("DATE", date);
        settingsRecordARMFile.addValue("client","--AugmentedRealityMirror-- Version: BETA.0");
        settingsRecordARMFile.addValue("authors", "Lee Meredith");
        settingsRecordARMFile.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
        settingsRecordARMFile.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
        settingsRecordARMFile.popTag();
        //client end about
        
        //version
        settingsRecordARMFile.addTag("version");
        settingsRecordARMFile.pushTag("version");
        
        settingsRecordARMFile.addValue("major", 0);
        settingsRecordARMFile.addValue("minor", 6);
        settingsRecordARMFile.popTag();
        //end version 
        
        //recordARM header
        settingsRecordARMFile.addTag("RECORD");
        settingsRecordARMFile.pushTag("RECORD", numAboutTags-1);
        settingsRecordARMFile.addValue("file", currentXmlFile);
        date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        settingsRecordARMFile.addValue("DATE", date);
        settingsRecordARMFile.addValue("SESSION", recordSession);
        settingsRecordARMFile.addValue("PARTICIPANT", recordParticipant);
        settingsRecordARMFile.addValue("AGE", recordAge);
        settingsRecordARMFile.addValue("AUDIO", recordAudio);
        
        settingsRecordARMFile.addValue("CAMERA", -1);
        settingsRecordARMFile.addValue("PAIN_SCORE", -1);
        settingsRecordARMFile.addValue("TIME", "-1");
        settingsRecordARMFile.addValue("POS_X", -1);
        settingsRecordARMFile.addValue("POS_Y", -1);
        settingsRecordARMFile.addValue("POS_Z", -1);
        
        settingsRecordARMFile.addValue("XAXIS_X", -1);
        settingsRecordARMFile.addValue("XAXIS_Y", -1);
        settingsRecordARMFile.addValue("XAXIS_Z", -1);
        
        settingsRecordARMFile.addValue("YAXIS_X", -1);
        settingsRecordARMFile.addValue("YAXIS_Y", -1);
        settingsRecordARMFile.addValue("YAXIS_Z", -1);
        
        settingsRecordARMFile.addValue("ZAXIS_X", -1);
        settingsRecordARMFile.addValue("ZAXIS_y", -1);
        settingsRecordARMFile.addValue("ZAXIS_Z", -1);
        
        settingsRecordARMFile.addValue("ENDRECORD", -1);
        
        settingsRecordARMFile.popTag();
        //record about
        
        settingsRecordARMFile.popTag(); //pop position
    }
    
    setRecordAddressTest = setRecordAddress = 0;
    setCameraAddress = 0;
    //get----------------------------------_
    isGetAll = true;
    if (isGetAll == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            getDate = settingsRecordARMFile.getValue("DATE","");
            getSession = settingsRecordARMFile.getValue("SESSION", "");
            getParticipant = settingsRecordARMFile.getValue("PARTICIPANT", "");
            getAge = settingsRecordARMFile.getValue("AGE", "");
            getAmputation = settingsRecordARMFile.getValue("AMPUTATION", "");
            getAudio = settingsRecordARMFile.getValue("AUDIO", "");
            
            int numberOfSavedCamera = settingsRecordARMFile.getNumTags("CAMERA");
            for(int i = 0; i < numberOfSavedCamera; i++){
                int myCamera;
                myCamera = settingsRecordARMFile.getValue("CAMERA", 0, i);// or ("X", 0)
                allCamera.push_back(myCamera);
            }
            
            int numberOfSavedPainScore = settingsRecordARMFile.getNumTags("PAIN_SCORE");
            for(int i = 0; i < numberOfSavedPainScore; i++){
                int myPainScore;
                myPainScore = settingsRecordARMFile.getValue("PAIN_SCORE", 0, i);
                allPainScores.push_back(myPainScore);
            }
            
            int numberOfSavedTime = settingsRecordARMFile.getNumTags("TIME");
            for(int i = 0; i < numberOfSavedTime; i++){
                string myTime;
                myTime = settingsRecordARMFile.getValue("TIME", " ", i);
                allTimes.push_back(myTime);
            }
            //position-------------------------_
            int numberOfSavedPosition_X = settingsRecordARMFile.getNumTags("POS_X");
            for(int i = 0; i < numberOfSavedPosition_X; i++){
                float myPosition_X;
                myPosition_X = settingsRecordARMFile.getValue("POS_X", 0.0, i);
                allPositions_X.push_back(myPosition_X);
            }
            int numberOfSavedPosition_Y = settingsRecordARMFile.getNumTags("POS_Y");
            for(int i = 0; i < numberOfSavedPosition_Y; i++){
                float myPosition_Y;
                myPosition_Y = settingsRecordARMFile.getValue("POS_Y", 0.0, i);
                allPositions_Y.push_back(myPosition_Y);
            }
            int numberOfSavedPosition_Z = settingsRecordARMFile.getNumTags("POS_Z");
            for(int i = 0; i < numberOfSavedPosition_Y; i++){
                float myPosition_Z;
                myPosition_Z = settingsRecordARMFile.getValue("POS_Z", 0.0, i);
                allPositions_Z.push_back(myPosition_Z);
            }
            //-------------position------------_
            //XAxis-------------------------_
            int numberOfSavedXAxis_X = settingsRecordARMFile.getNumTags("XAXIS_X");
            for(int i = 0; i < numberOfSavedXAxis_X; i++){           
                float myXAxis_X;
                myXAxis_X = settingsRecordARMFile.getValue("XAXIS_X", 0.0, i);
                allXAxis_X.push_back(myXAxis_X);
            }
            int numberOfSavedXAxis_Y = settingsRecordARMFile.getNumTags("XAXIS_Y");
            for(int i = 0; i < numberOfSavedXAxis_Y; i++){           
                float myXAxis_Y;
                myXAxis_Y = settingsRecordARMFile.getValue("XAXIS_Y", 0.0, i);
                allXAxis_Y.push_back(myXAxis_Y);
            }
            int numberOfSavedXAxis_Z = settingsRecordARMFile.getNumTags("XAXIS_Z");
            for(int i = 0; i < numberOfSavedXAxis_Z; i++){           
                float myXAxis_Z;
                myXAxis_Z = settingsRecordARMFile.getValue("XAXIS_Z", 0.0, i);
                allXAxis_Z.push_back(myXAxis_Z);
            }
            //------------XAxis-------------_
            //YAxis-------------------------_
            int numberOfSavedYAxis_X = settingsRecordARMFile.getNumTags("YAXIS_X");
            for(int i = 0; i < numberOfSavedYAxis_X; i++){
                float myYAxis_X;
                myYAxis_X = settingsRecordARMFile.getValue("YAXIS_X", 0.0, i);
                allYAxis_X.push_back(myYAxis_X);
            }
            int numberOfSavedYAxis_Y = settingsRecordARMFile.getNumTags("YAXIS_Y");
            for(int i = 0; i < numberOfSavedYAxis_Y; i++){
                float myYAxis_Y;
                myYAxis_Y = settingsRecordARMFile.getValue("YAXIS_Y", 0.0, i);
                allYAxis_Y.push_back(myYAxis_Y);
            }
            int numberOfSavedYAxis_Z = settingsRecordARMFile.getNumTags("YAXIS_Z");
            for(int i = 0; i < numberOfSavedYAxis_Z; i++){
                float myYAxis_Z;
                myYAxis_Z = settingsRecordARMFile.getValue("YAXIS_Z", 0.0, i);
                allYAxis_Z.push_back(myYAxis_Z);
            }
            //-------------YAxis------------_
            //ZAxis-------------------------_
            int numberOfSavedZAxis_X = settingsRecordARMFile.getNumTags("ZAXIS_X");
            for(int i = 0; i < numberOfSavedZAxis_X; i++){
                float myZAxis_X;
                myZAxis_X = settingsRecordARMFile.getValue("ZAXIS_X", 0.0, i);
                allZAxis_X.push_back(myZAxis_X);
            }
            int numberOfSavedZAxis_Y = settingsRecordARMFile.getNumTags("ZAXIS_Y");
            for(int i = 0; i < numberOfSavedZAxis_Y; i++){
                float myZAxis_Y;
                myZAxis_Y = settingsRecordARMFile.getValue("ZAXIS_Y", 0.0, i);
                allZAxis_Y.push_back(myZAxis_Y);
            }
            int numberOfSavedZAxis_Z = settingsRecordARMFile.getNumTags("ZAXIS_Z");
            for(int i = 0; i < numberOfSavedZAxis_Z; i++){
                float myZAxis_Z;
                myZAxis_Z = settingsRecordARMFile.getValue("ZAXIS_Z", 0.0, i);
                allZAxis_Z.push_back(myZAxis_Z);
            }
            //-----------ZAxis--------------_
            
            int numberOfSavedEndRec = settingsRecordARMFile.getNumTags("ENDRECORD");
            if (numberOfSavedEndRec > 0) {
                lastTagNumberEndRec = settingsRecordARMFile.getValue("ENDRECORD", 0.0, numberOfSavedEndRec-1);
            }
            settingsRecordARMFile.popTag(); //pop position
        }
        isGetAll = false;
    }
    
    setGetRecordAddress = setGetCameraAddress = setGetTimesAddress = setGetPainScoreAddress = 0;
    isCameraAddressEnd = isTimesAddressEnd = isPositionsAddressEnd = isXAxisAddressEnd = isYAxisAddressEnd = isZAxisAddressEnd = false;
    if (allCamera.size() > 0) {
        guiDateTime = getDate;
        guiSession = getSession;
        guiParticipant = getParticipant;
        guiAge = getAge;
        guiAmputation = getAmputation;
        guiAudio = getAudio;
        
        setGetRecordAddress = 0;
        //setGetRecordAddressAll = recRecordARM_0.setEventssAddress;
        if (setGetRecordAddressAll >= allCamera.size()) {
            setGetCameraAddress = allCamera.size()-1;
        }
        if (setGetRecordAddressAll >= allPainScores.size()) {
            setGetPainScoreAddress = allPainScores.size()-1;
        }
        if (setGetRecordAddressAll >= allTimes.size()) {
            setGetTimesAddress = allTimes.size()-1;
        }
        if (setGetRecordAddressAll >= allPositions_X.size()) {
            setGetPositionsAddress_X = allPositions_X.size()-1;
        }
        if (setGetRecordAddressAll >= allPositions_Y.size()) {
            setGetPositionsAddress_Y = allPositions_Y.size()-1;
        }
        if (setGetRecordAddressAll >= allPositions_Y.size()) {
            setGetPositionsAddress_Z = allPositions_Y.size()-1;
        }
        if (setGetRecordAddressAll >= allXAxis_X.size()) {
            setGetXAxisXAddress = allXAxis_X.size()-1;
        }
        if (setGetRecordAddressAll >= allXAxis_Y.size()) {
            setGetXAxisYAddress = allXAxis_Y.size()-1;
        }
        if (setGetRecordAddressAll >= allXAxis_Z.size()) {
            setGetXAxisZAddress = allXAxis_Z.size()-1;
        }
        if (setGetRecordAddressAll >= allYAxis_X.size()) {
            setGetYAxisXAddress = allYAxis_X.size()-1;
        }
        if (setGetRecordAddressAll >= allYAxis_Y.size()) {
            setGetYAxisYAddress = allYAxis_Y.size()-1;
        }
        if (setGetRecordAddressAll >= allYAxis_Z.size()) {
            setGetYAxisZAddress = allYAxis_Z.size()-1;
        }
        if (setGetRecordAddressAll >= allZAxis_X.size()) {
            setGetZAxisXAddress = allZAxis_X.size()-1;
        }
        if (setGetRecordAddressAll >= allZAxis_Y.size()) {
            setGetZAxisYAddress = allZAxis_Y.size()-1;
        }
        if (setGetRecordAddressAll >= allZAxis_Z.size()) {
            setGetZAxisZAddress = allZAxis_Z.size()-1;
        }
        if (setGetRecordAddressAll >= allScale.size()) {
            setGetScaleAddress = allScale.size()-1;
        }
        
        guiRecordVal = recordAddress;
        if (allCamera.size() > 0)guiCameraVal = allCamera[setGetCameraAddress];
        if (allPainScores.size() > 0)guiPainScoreVal = allPainScores[setGetPainScoreAddress];
        if (allTimes.size() > 0)guiTimeVal = allTimes[setGetTimesAddress];
        if (allPositions_X.size() > 0)guiPositionXVal = allPositions_X[setGetPositionsAddress_X];
        if (allPositions_Y.size() > 0)guiPositionYVal = allPositions_Y[setGetPositionsAddress_Y];
        if (allPositions_Z.size() > 0)guiPositionZVal = allPositions_Z[setGetPositionsAddress_Z];
        if (allXAxis_X.size() > 0)guiXAxisXVal = allXAxis_X[setGetXAxisXAddress];
        if (allXAxis_Y.size() > 0)guiXAxisYVal = allXAxis_Y[setGetXAxisYAddress];
        if (allXAxis_Z.size() > 0)guiXAxisZVal = allXAxis_Z[setGetXAxisZAddress];
        if (allYAxis_X.size() > 0)guiYAxisXVal = allYAxis_X[setGetYAxisXAddress];
        if (allYAxis_Y.size() > 0)guiYAxisYVal = allYAxis_Y[setGetYAxisYAddress];
        if (allYAxis_Z.size() > 0)guiYAxisZVal = allYAxis_Z[setGetYAxisZAddress];
        if (allZAxis_X.size() > 0)guiZAxisXVal = allZAxis_X[setGetZAxisXAddress];
        if (allZAxis_Y.size() > 0)guiZAxisYVal = allZAxis_Y[setGetZAxisYAddress];
        if (allZAxis_Z.size() > 0)guiZAxisZVal = allZAxis_Z[setGetZAxisZAddress];
        if (allScale.size() > 0)guiScaleVal = allScale[setGetScaleAddress];
        guiEndRecVal = lastTagNumberEndRec;
    }
    //---------------get-------------------_
    
    //edit----------------------------_
    isSaveAll = isSaveEvents = isStartEvents = isSetRecord = false;
    isLogging = isLoggingKey = false;
    //------------edit----------------_
}

//--------------------------------------------------------------
void recRecordARM::update(string xmlFile){
    
    if (xmlFile != xmlFileTest) {
        currentXmlFile = "record/address/";
        currentXmlFile += xmlFile;
        settingsDirectory = currentXmlFile;
        
        int posLastSlash = settingsDirectory.rfind("/");
        if( posLastSlash > 0) settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
        else settingsDirectory = "";
        
        int posLastDot = currentXmlFile.rfind(".xml");
        if (posLastDot < 1) {
            currentXmlFile += ".xml";
        }
        //currentXmlFile += "/";
        
        message = "loading ";
        message += currentXmlFile;
        
        if( settingsRecordARMFile.loadFile(currentXmlFile) ){
            message = currentXmlFile;
            message += " loaded!";
        }else{
            message = "unable to load ";
            message += currentXmlFile;
        }
        xmlFileTest = xmlFile;
    }
    
    //get----------------------------------_
    if (isGetAll == true) {
        setGetRecordAddressAll = 0;
        allCamera.clear();
        allPainScores.clear();
        allTimes.clear();
        allPositions_X.clear();
        allPositions_Y.clear();
        allPositions_Z.clear();
        allXAxis_X.clear();
        allXAxis_Y.clear();
        allXAxis_Z.clear();
        allYAxis_X.clear();
        allYAxis_Y.clear();
        allYAxis_Z.clear();
        allZAxis_X.clear();
        allZAxis_Y.clear();
        allZAxis_Z.clear();
        allScale.clear();
        if (setRecordAddress != setRecordAddressTest) {
            setRecordAddressTest = setRecordAddress;
            isSetRecordAddressNew = true;
        }
        if (setRecordAddress == setRecordAddressTest){
            isSetRecordAddressNew = false;
        }
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            getDate = settingsRecordARMFile.getValue("DATE","");
            getSession = settingsRecordARMFile.getValue("SESSION", "");
            getParticipant = settingsRecordARMFile.getValue("PARTICIPANT", "");
            getAge = settingsRecordARMFile.getValue("AGE", "");
            getAmputation = settingsRecordARMFile.getValue("AMPUTATION", "");
            getAudio = settingsRecordARMFile.getValue("AUDIO", "");
            
            int numberOfSavedCamera = settingsRecordARMFile.getNumTags("CAMERA");
            for(int i = 0; i < numberOfSavedCamera; i++){
                int myCamera;
                myCamera = settingsRecordARMFile.getValue("CAMERA", 0, i);// or ("X", 0)
                allCamera.push_back(myCamera);
            }
            
            int numberOfSavedPainScore = settingsRecordARMFile.getNumTags("PAIN_SCORE");
            for(int i = 0; i < numberOfSavedPainScore; i++){
                int myPainScore;
                myPainScore = settingsRecordARMFile.getValue("PAIN_SCORE", 0, i);
                allPainScores.push_back(myPainScore);
            }
            
            int numberOfSavedTime = settingsRecordARMFile.getNumTags("TIME");
            for(int i = 0; i < numberOfSavedTime; i++){
                string myTime;
                myTime = settingsRecordARMFile.getValue("TIME", " ", i);
                allTimes.push_back(myTime);
            }
            //position-------------------------_
            int numberOfSavedPosition_X = settingsRecordARMFile.getNumTags("POS_X");
            for(int i = 0; i < numberOfSavedPosition_X; i++){
                float myPosition_X;
                myPosition_X = settingsRecordARMFile.getValue("POS_X", 0.0, i);
                allPositions_X.push_back(myPosition_X);
            }
            int numberOfSavedPosition_Y = settingsRecordARMFile.getNumTags("POS_Y");
            for(int i = 0; i < numberOfSavedPosition_Y; i++){
                float myPosition_Y;
                myPosition_Y = settingsRecordARMFile.getValue("POS_Y", 0.0, i);
                allPositions_Y.push_back(myPosition_Y);
            }
            int numberOfSavedPosition_Z = settingsRecordARMFile.getNumTags("POS_Z");
            for(int i = 0; i < numberOfSavedPosition_Y; i++){
                float myPosition_Z;
                myPosition_Z = settingsRecordARMFile.getValue("POS_Z", 0.0, i);
                allPositions_Z.push_back(myPosition_Z);
            }
            //-------------position------------_
            //XAxis-------------------------_
            int numberOfSavedXAxis_X = settingsRecordARMFile.getNumTags("XAXIS_X");
            for(int i = 0; i < numberOfSavedXAxis_X; i++){           
                float myXAxis_X;
                myXAxis_X = settingsRecordARMFile.getValue("XAXIS_X", 0.0, i);
                allXAxis_X.push_back(myXAxis_X);
            }
            int numberOfSavedXAxis_Y = settingsRecordARMFile.getNumTags("XAXIS_Y");
            for(int i = 0; i < numberOfSavedXAxis_Y; i++){           
                float myXAxis_Y;
                myXAxis_Y = settingsRecordARMFile.getValue("XAXIS_Y", 0.0, i);
                allXAxis_Y.push_back(myXAxis_Y);
            }
            int numberOfSavedXAxis_Z = settingsRecordARMFile.getNumTags("XAXIS_Z");
            for(int i = 0; i < numberOfSavedXAxis_Z; i++){           
                float myXAxis_Z;
                myXAxis_Z = settingsRecordARMFile.getValue("XAXIS_Z", 0.0, i);
                allXAxis_Z.push_back(myXAxis_Z);
            }
            //------------XAxis-------------_
            //YAxis-------------------------_
            int numberOfSavedYAxis_X = settingsRecordARMFile.getNumTags("YAXIS_X");
            for(int i = 0; i < numberOfSavedYAxis_X; i++){
                float myYAxis_X;
                myYAxis_X = settingsRecordARMFile.getValue("YAXIS_X", 0.0, i);
                allYAxis_X.push_back(myYAxis_X);
            }
            int numberOfSavedYAxis_Y = settingsRecordARMFile.getNumTags("YAXIS_Y");
            for(int i = 0; i < numberOfSavedYAxis_Y; i++){
                float myYAxis_Y;
                myYAxis_Y = settingsRecordARMFile.getValue("YAXIS_Y", 0.0, i);
                allYAxis_Y.push_back(myYAxis_Y);
            }
            int numberOfSavedYAxis_Z = settingsRecordARMFile.getNumTags("YAXIS_Z");
            for(int i = 0; i < numberOfSavedYAxis_Z; i++){
                float myYAxis_Z;
                myYAxis_Z = settingsRecordARMFile.getValue("YAXIS_Z", 0.0, i);
                allYAxis_Z.push_back(myYAxis_Z);
            }
            //-------------YAxis------------_
            //ZAxis-------------------------_
            int numberOfSavedZAxis_X = settingsRecordARMFile.getNumTags("ZAXIS_X");
            for(int i = 0; i < numberOfSavedZAxis_X; i++){
                float myZAxis_X;
                myZAxis_X = settingsRecordARMFile.getValue("ZAXIS_X", 0.0, i);
                allZAxis_X.push_back(myZAxis_X);
            }
            int numberOfSavedZAxis_Y = settingsRecordARMFile.getNumTags("ZAXIS_Y");
            for(int i = 0; i < numberOfSavedZAxis_Y; i++){
                float myZAxis_Y;
                myZAxis_Y = settingsRecordARMFile.getValue("ZAXIS_Y", 0.0, i);
                allZAxis_Y.push_back(myZAxis_Y);
            }
            int numberOfSavedZAxis_Z = settingsRecordARMFile.getNumTags("ZAXIS_Z");
            for(int i = 0; i < numberOfSavedZAxis_Z; i++){
                float myZAxis_Z;
                myZAxis_Z = settingsRecordARMFile.getValue("ZAXIS_Z", 0.0, i);
                allZAxis_Z.push_back(myZAxis_Z);
            }
            //-----------ZAxis--------------_
            int numberOfSavedScale = settingsRecordARMFile.getNumTags("NOTHING");
            for(int i = 0; i < numberOfSavedScale; i++){
                float myScale;
                myScale = settingsRecordARMFile.getValue("NOTHING", 0.0, i);
                allScale.push_back(myScale);
            }
            
            int numberOfSavedEndRec = settingsRecordARMFile.getNumTags("ENDRECORD");
            if (numberOfSavedEndRec > 0) {
                lastTagNumberEndRec = settingsRecordARMFile.getValue("ENDRECORD", 0.0, numberOfSavedEndRec-1);
            }
            settingsRecordARMFile.popTag(); //pop position
        }
        isGetAll = false;
    }
    
    if (allCamera.size() > 0) {
        guiDateTime = getDate;
        guiSession = getSession;
        guiParticipant = getParticipant;
        guiAge = getAge;
        guiAmputation = getAmputation;
        guiAudio = getAudio;
        
        setGetRecordAddress = 0;
        
        if (setGetRecordAddressAll >= allCamera.size()-1) {
            setGetCameraAddress = allCamera.size()-1;
            isCameraAddressEnd = true;
        }else {
            setGetCameraAddress = setGetRecordAddressAll;
            isCameraAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allPainScores.size()-1) {
            setGetPainScoreAddress = allPainScores.size()-1;
            isPainScoresAddressEnd = true;
        }else {
            setGetPainScoreAddress = setGetRecordAddressAll;
            isPainScoresAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allTimes.size()-1) {//-1 is to give the stop Color
            setGetTimesAddress = allTimes.size()-1;
            isTimesAddressEnd = true;
        }else {
            setGetTimesAddress = setGetRecordAddressAll;
            isTimesAddressEnd = false;
        }
        
        getAllPositionsXSize = allPositions_X.size();
        if (setGetRecordAddressAll >= allPositions_X.size()-1) {
            setGetPositionsAddress_X = allPositions_X.size()-1;
            isPositionsAddressEnd = true;
        }else {
            setGetPositionsAddress_X = setGetRecordAddressAll;
            isPositionsAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allPositions_Y.size()-1) {
            setGetPositionsAddress_Y = allPositions_Y.size()-1;
            isPositionsAddressEnd = true;
        }else {
            setGetPositionsAddress_Y = setGetRecordAddressAll;
            isPositionsAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allPositions_Z.size()-1) {
            setGetPositionsAddress_Z = allPositions_Z.size()-1;
            isPositionsAddressEnd = true;
        }else {
            setGetPositionsAddress_Z = setGetRecordAddressAll;
            isPositionsAddressEnd = false;
        }
        
        if (setGetRecordAddressAll >= allXAxis_X.size()-1) {
            setGetXAxisXAddress = allXAxis_X.size()-1;
            isXAxisAddressEnd = true;
        }else {
            setGetXAxisXAddress = setGetRecordAddressAll;
            isXAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allXAxis_Y.size()-1) {
            setGetXAxisYAddress = allXAxis_Y.size()-1;
            isXAxisAddressEnd = true;
        }else {
            setGetXAxisYAddress = setGetRecordAddressAll;
            isXAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allXAxis_Z.size()-1) {
            setGetXAxisZAddress = allXAxis_Z.size()-1;
            isXAxisAddressEnd = true;
        }else {
            setGetXAxisZAddress = setGetRecordAddressAll;
            isXAxisAddressEnd = false;
        }
        
        if (setGetRecordAddressAll >= allYAxis_X.size()-1) {
            setGetYAxisXAddress = allYAxis_X.size()-1;
            isYAxisAddressEnd = true;
        }else {
            setGetYAxisXAddress = setGetRecordAddressAll;
            isYAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allYAxis_Y.size()-1) {
            setGetYAxisYAddress = allYAxis_Y.size()-1;
            isYAxisAddressEnd = true;
        }else {
            setGetYAxisYAddress = setGetRecordAddressAll;
            isYAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allYAxis_Z.size()-1) {
            setGetYAxisZAddress = allYAxis_Z.size()-1;
            isYAxisAddressEnd = true;
        }else {
            setGetYAxisZAddress = setGetRecordAddressAll;
            isYAxisAddressEnd = false;
        }
        
        if (setGetRecordAddressAll >= allZAxis_X.size()-1) {
            setGetZAxisXAddress = allZAxis_X.size()-1;
            isZAxisAddressEnd = true;
        }else {
            setGetZAxisXAddress = setGetRecordAddressAll;
            isZAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allZAxis_Y.size()-1) {
            setGetZAxisYAddress = allZAxis_Y.size()-1;
            isZAxisAddressEnd = true;
        }else {
            setGetZAxisYAddress = setGetRecordAddressAll;
            isZAxisAddressEnd = false;
        }
        if (setGetRecordAddressAll >= allZAxis_Z.size()-1) {
            setGetZAxisZAddress = allZAxis_Z.size()-1;
            isZAxisAddressEnd = true;
        }else {
            setGetZAxisZAddress = setGetRecordAddressAll;
            isZAxisAddressEnd = false;
        }
                
        guiRecordVal = recordAddress;
        if (allCamera.size() > 0)guiCameraVal = allCamera[setGetCameraAddress];
        if (allPainScores.size() > 0)guiPainScoreVal = allPainScores[setGetPainScoreAddress];
        if (allTimes.size() > 0)guiTimeVal = allTimes[setGetTimesAddress];
        if (allPositions_X.size() > 0)guiPositionXVal = allPositions_X[setGetPositionsAddress_X];
        if (allPositions_Y.size() > 0)guiPositionYVal = allPositions_Y[setGetPositionsAddress_Y];
        if (allPositions_Z.size() > 0)guiPositionZVal = allPositions_Z[setGetPositionsAddress_Z];
        if (allXAxis_X.size() > 0)guiXAxisXVal = allXAxis_X[setGetXAxisXAddress];
        if (allXAxis_Y.size() > 0)guiXAxisYVal = allXAxis_Y[setGetXAxisYAddress];
        if (allXAxis_Z.size() > 0)guiXAxisZVal = allXAxis_Z[setGetXAxisZAddress];
        if (allYAxis_X.size() > 0)guiYAxisXVal = allYAxis_X[setGetYAxisXAddress];
        if (allYAxis_Y.size() > 0)guiYAxisYVal = allYAxis_Y[setGetYAxisYAddress];
        if (allYAxis_Z.size() > 0)guiYAxisZVal = allYAxis_Z[setGetYAxisZAddress];
        if (allZAxis_X.size() > 0)guiZAxisXVal = allZAxis_X[setGetZAxisXAddress];
        if (allZAxis_Y.size() > 0)guiZAxisYVal = allZAxis_Y[setGetZAxisYAddress];
        if (allZAxis_Z.size() > 0)guiZAxisZVal = allZAxis_Z[setGetZAxisZAddress];
        if (allScale.size() > 0)guiScaleVal = allScale[setGetScaleAddress];
        guiEndRecVal = lastTagNumberEndRec;
    }
    //---------------get-------------------_
    
    //edit----------------------------_
    if(isLogging == true){
        int myCamera;
        myCamera = recordCamera;
        camera.push_back(myCamera);
        
        int myPainScore;
        recordPainScore = 0;
        myPainScore = recordPainScore;
        painScore.push_back(myPainScore);
        
        string myTime;
        myTime = recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(ofGetElapsedTimeMillis());
        time.push_back(myTime);
        
        float myPosition_X;
        myPosition_X = recordPosition_X;
        position_X.push_back(myPosition_X);
        float myPosition_Y;
        myPosition_Y = recordPosition_Y;
        position_Y.push_back(myPosition_Y);
        float myPosition_Z;
        myPosition_Z = recordPosition_Z;
        position_Z.push_back(myPosition_Z);
        
        float myXAxis_X;
        myXAxis_X = recordXAxis_X;
        xAxis_X.push_back(myXAxis_X);
        float myXAxis_Y;
        myXAxis_Y = recordXAxis_Y;
        xAxis_Y.push_back(myXAxis_Y);
        float myXAxis_Z;
        myXAxis_Z = recordXAxis_Z;
        xAxis_Z.push_back(myXAxis_Z);
        
        float myYAxis_X;
        myYAxis_X = recordYAxis_X;
        yAxis_X.push_back(myYAxis_X);
        float myYAxis_Y;
        myYAxis_Y = recordYAxis_Y;
        yAxis_Y.push_back(myYAxis_Y);
        float myYAxis_Z;
        myYAxis_Z = recordYAxis_Z;
        yAxis_Z.push_back(myYAxis_Z);
        
        float myZAxis_X;
        myZAxis_X = recordZAxis_X;
        zAxis_X.push_back(myZAxis_X);
        float myZAxis_Y;
        myZAxis_Y = recordZAxis_Y;
        zAxis_Y.push_back(myZAxis_Y);
        float myZAxis_Z;
        myZAxis_Z = recordZAxis_Z;
        zAxis_Z.push_back(myZAxis_Z);
        
        isStartEvents = true;
        isEventsTest = false;
        isSaveAll = true;
        isLogging = false;
    }
    
    if (isSaveAll == true) {
        settingsRecordARMFile.saveFile(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        isSaveAll = false;
    }
    if (isSetRecord == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            settingsRecordARMFile.setValue("DATE", date);
            settingsRecordARMFile.setValue("SESSION", recordSession);
            settingsRecordARMFile.setValue("PARTICIPANT", recordParticipant);
            settingsRecordARMFile.setValue("AGE", recordAge);
            settingsRecordARMFile.setValue("AMPUTATION", recordAmputation);
            settingsRecordARMFile.setValue("AUDIO", recordAudio);
            settingsRecordARMFile.popTag();
        }
        isSetRecord = false;
    }
    
    if (isStartEvents == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            int myCamera;
            myCamera = settingsRecordARMFile.getValue("CAMERA", 0, 0);
            if (myCamera == -1) {
                settingsRecordARMFile.removeTag("CAMERA", 0);
                settingsRecordARMFile.addValue("CAMERA", camera[0]);
            }else {
                settingsRecordARMFile.addValue("CAMERA", camera[0]);
            }
            int myPainScores;
            myPainScores = settingsRecordARMFile.getValue("PAIN_SCORE", 0, 0);
            if (myPainScores == -1) {
                settingsRecordARMFile.removeTag("PAIN_SCORE", 0);
                settingsRecordARMFile.addValue("PAIN_SCORE", painScore[0]);
            }else {
                settingsRecordARMFile.addValue("PAIN_SCORE", painScore[0]);
            }
            string myTimes;
            myTimes = settingsRecordARMFile.getValue("TIME", " ", 0);
            if (myTimes == "-1") {
                settingsRecordARMFile.removeTag("TIME", 0);
                settingsRecordARMFile.addValue("TIME", time[0]);
            }else {
                settingsRecordARMFile.addValue("TIME", time[0]);
            }
            
            float myPositions_X;
            myPositions_X = settingsRecordARMFile.getValue("POS_X", 0.0, 0);
            if (myPositions_X == -1) {
                settingsRecordARMFile.removeTag("POS_X", 0);
                settingsRecordARMFile.addValue("POS_X", position_X[0]);
            }else {
                settingsRecordARMFile.addValue("POS_X", position_X[0]);
            }
            float myPositions_Y;
            myPositions_Y = settingsRecordARMFile.getValue("POS_Y", 0.0, 0);
            if (myPositions_Y == -1) {
                settingsRecordARMFile.removeTag("POS_Y", 0);
                settingsRecordARMFile.addValue("POS_Y", position_Y[0]);
            }else {
                settingsRecordARMFile.addValue("POS_Y", position_Y[0]);
            }
            float myPositions_Z;
            myPositions_Z = settingsRecordARMFile.getValue("POS_Z", 0.0, 0);
            if (myPositions_Z == -1) {
                settingsRecordARMFile.removeTag("POS_Z", 0);
                settingsRecordARMFile.addValue("POS_Z", position_Z[0]);
            }else {
                settingsRecordARMFile.addValue("POS_Z", position_Z[0]);
            }
            
            //XAxis-------------------------_
            float myXAxis_X;
            myXAxis_X = settingsRecordARMFile.getValue("XAXIS_X", 0.0, 0);
            if (myXAxis_X == -1) {
                settingsRecordARMFile.removeTag("XAXIS_X", 0);
                settingsRecordARMFile.addValue("XAXIS_X", xAxis_X[0]);
            }else {
                settingsRecordARMFile.addValue("XAXIS_X", xAxis_X[0]);
            }
            float myXAxis_Y;
            myXAxis_Y = settingsRecordARMFile.getValue("XAXIS_Y", 0.0, 0);
            if (myXAxis_Y == -1) {
                settingsRecordARMFile.removeTag("XAXIS_Y", 0);
                settingsRecordARMFile.addValue("XAXIS_Y", xAxis_Y[0]);
            }else {
                settingsRecordARMFile.addValue("XAXIS_Y", xAxis_Y[0]);
            }
            float myXAxis_Z;
            myXAxis_Z = settingsRecordARMFile.getValue("XAXIS_Z", 0.0, 0);
            if (myXAxis_Z == -1) {
                settingsRecordARMFile.removeTag("XAXIS_Z", 0);
                settingsRecordARMFile.addValue("XAXIS_Z", xAxis_Z[0]);
            }else {
                settingsRecordARMFile.addValue("XAXIS_Z", xAxis_Z[0]);
            }
            //-------------XAxis------------_
            
            //YAxis-------------------------_
            float myYAxis_X;
            myYAxis_X = settingsRecordARMFile.getValue("YAXIS_X", 0.0, 0);
            if (myYAxis_X == -1) {
                settingsRecordARMFile.removeTag("YAXIS_X", 0);
                settingsRecordARMFile.addValue("YAXIS_X", yAxis_X[0]);
            }else {
                settingsRecordARMFile.addValue("YAXIS_X", yAxis_X[0]);
            }
            float myYAxis_Y;
            myYAxis_Y = settingsRecordARMFile.getValue("YAXIS_Y", 0.0, 0);
            if (myYAxis_Y == -1) {
                settingsRecordARMFile.removeTag("YAXIS_Y", 0);
                settingsRecordARMFile.addValue("YAXIS_Y", yAxis_Y[0]);
            }else {
                settingsRecordARMFile.addValue("YAXIS_Y", yAxis_Y[0]);
            }
            float myYAxis_Z;
            myYAxis_Z = settingsRecordARMFile.getValue("YAXIS_Z", 0.0, 0);
            if (myYAxis_Z == -1) {
                settingsRecordARMFile.removeTag("YAXIS_Z", 0);
                settingsRecordARMFile.addValue("YAXIS_Z", yAxis_Z[0]);
            }else {
                settingsRecordARMFile.addValue("YAXIS_Z", yAxis_Z[0]);
            }
            //-------------YAxis------------_
            
            //ZAxis-------------------------_
            float myZAxis_X;
            myZAxis_X = settingsRecordARMFile.getValue("ZAXIS_X", 0.0, 0);
            if (myZAxis_X == -1) {
                settingsRecordARMFile.removeTag("ZAXIS_X", 0);
                settingsRecordARMFile.addValue("ZAXIS_X", zAxis_X[0]);
            }else {
                settingsRecordARMFile.addValue("ZAXIS_X", zAxis_X[0]);
            }
            float myZAxis_Y;
            myZAxis_Y = settingsRecordARMFile.getValue("ZAXIS_Y", 0.0, 0);
            if (myZAxis_Y == -1) {
                settingsRecordARMFile.removeTag("ZAXIS_Y", 0);
                settingsRecordARMFile.addValue("ZAXIS_Y", zAxis_Y[0]);
            }else {
                settingsRecordARMFile.addValue("ZAXIS_Y", zAxis_Y[0]);
            }
            float myZAxis_Z;
            myZAxis_Z = settingsRecordARMFile.getValue("ZAXIS_Z", 0.0, 0);
            if (myZAxis_Z == -1) {
                settingsRecordARMFile.removeTag("ZAXIS_Z", 0);
                settingsRecordARMFile.addValue("ZAXIS_Z", zAxis_Z[0]);
            }else {
                settingsRecordARMFile.addValue("ZAXIS_Z", zAxis_Z[0]);
            }
            //-------------ZAxis------------_
            
            settingsRecordARMFile.popTag(); //pop position
        }
        
        settingsRecordARMFile.saveFile(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        
        camera.clear();
        painScore.clear();
        time.clear();
        position_X.clear();
        position_Y.clear();
        position_Z.clear();
        xAxis_X.clear();
        xAxis_Y.clear();
        xAxis_Z.clear();
        yAxis_X.clear();
        yAxis_Y.clear();
        yAxis_Z.clear();
        zAxis_X.clear();
        zAxis_Y.clear();
        zAxis_Z.clear();
        /*
        allCamera.clear();
        allPainScores.clear();
        allTimes.clear();
        allPositions_X.clear();
        allPositions_Y.clear();
        allPositions_Z.clear();
        allXAxis_X.clear();
        allXAxis_Y.clear();
        allXAxis_Z.clear();
        allYAxis_X.clear();
        allYAxis_Y.clear();
        allYAxis_Z.clear();
        allZAxis_X.clear();
        allZAxis_Y.clear();
        allZAxis_Z.clear();
        allScale.clear();
         */
        isStartEvents = false;
    }
    
    if (isSaveEvents == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            for(int i = 0; i < camera.size(); i++){
                settingsRecordARMFile.addValue("CAMERA", camera[i]);
            }
            for(int i = 0; i < painScore.size(); i++){
                settingsRecordARMFile.addValue("PAIN_SCORE", painScore[i]);
            }
            for(int i = 0; i < time.size(); i++){
                settingsRecordARMFile.addValue("TIME", time[i]);
            }
            for(int i = 0; i < position_X.size(); i++){
                settingsRecordARMFile.addValue("POS_X", position_X[i]);
            }
            for(int i = 0; i < position_Y.size(); i++){
                settingsRecordARMFile.addValue("POS_Y", position_Y[i]);
            }
            for(int i = 0; i < position_X.size(); i++){
                settingsRecordARMFile.addValue("POS_Z", position_X[i]);
            }
            //XAxis-------------------------_
            for(int i = 0; i < xAxis_X.size(); i++){
                settingsRecordARMFile.addValue("XAXIS_X", xAxis_X[i]);
            }
            for(int i = 0; i < xAxis_Y.size(); i++){
                settingsRecordARMFile.addValue("XAXIS_Y", xAxis_Y[i]);
            }
            for(int i = 0; i < xAxis_Z.size(); i++){
                settingsRecordARMFile.addValue("XAXIS_Z", xAxis_Z[i]);
            }
            //-------------XAxis------------_
            //YAxis-------------------------_
            for(int i = 0; i < yAxis_X.size(); i++){
                settingsRecordARMFile.addValue("YAXIS_X", yAxis_X[i]);
            }
            for(int i = 0; i < yAxis_Y.size(); i++){
                settingsRecordARMFile.addValue("YAXIS_Y", yAxis_Y[i]);
            }
            for(int i = 0; i < yAxis_Z.size(); i++){
                settingsRecordARMFile.addValue("YAXIS_Z", yAxis_Z[i]);
            }
            //-------------YAxis------------_
            //ZAxis-------------------------_
            for(int i = 0; i < zAxis_X.size(); i++){
                settingsRecordARMFile.addValue("ZAXIS_X", zAxis_X[i]);
            }
            for(int i = 0; i < zAxis_Y.size(); i++){
                settingsRecordARMFile.addValue("ZAXIS_Y", zAxis_Y[i]);
            }
            for(int i = 0; i < zAxis_Z.size(); i++){
                settingsRecordARMFile.addValue("ZAXIS_Z", zAxis_Z[i]);
            }
            //-------------ZAxis------------_
            settingsRecordARMFile.popTag(); //pop position
        }
        settingsRecordARMFile.saveFile(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        
        camera.clear();
        painScore.clear();
        time.clear();
        position_X.clear();
        position_Y.clear();
        position_Z.clear();
        xAxis_X.clear();
        xAxis_Y.clear();
        xAxis_Z.clear();
        yAxis_X.clear();
        yAxis_Y.clear();
        yAxis_Z.clear();
        zAxis_X.clear();
        zAxis_Y.clear();
        zAxis_Z.clear();
        isSaveEvents = false;
    }
    
    if (clearDoc == true) {
        
        camera.clear();
        painScore.clear();
        time.clear();
        position_X.clear();
        position_Y.clear();
        position_Z.clear();
        xAxis_X.clear();
        xAxis_Y.clear();
        xAxis_Z.clear();
        yAxis_X.clear();
        yAxis_Y.clear();
        yAxis_Z.clear();
        zAxis_X.clear();
        zAxis_Y.clear();
        zAxis_Z.clear();
        
        allCamera.clear();
        allPainScores.clear();
        allTimes.clear();
        allPositions_X.clear();
        allPositions_Y.clear();
        allPositions_Z.clear();
        allXAxis_X.clear();
        allXAxis_Y.clear();
        allXAxis_Z.clear();
        allYAxis_X.clear();
        allYAxis_Y.clear();
        allYAxis_Z.clear();
        allZAxis_X.clear();
        allZAxis_Y.clear();
        allZAxis_Z.clear();
        allScale.clear();
        
        settingsRecordARMFile.clear();
        
        int numAboutTags = settingsRecordARMFile.getNumTags("about");
        
        if(numAboutTags == 0){
            //client about
            settingsRecordARMFile.addTag("about");
            settingsRecordARMFile.pushTag("about");
            settingsRecordARMFile.addValue("file", currentXmlFile);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            settingsRecordARMFile.addValue("DATE", date);
            settingsRecordARMFile.addValue("client","--diamondsAndCode-- Version: BETA.0");
            settingsRecordARMFile.addValue("authors", "Lee Meredith");
            settingsRecordARMFile.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
            settingsRecordARMFile.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
            settingsRecordARMFile.popTag();
            //client end about
            
            //version
            settingsRecordARMFile.addTag("version");
            settingsRecordARMFile.pushTag("version");
            
            settingsRecordARMFile.addValue("major", 0);
            settingsRecordARMFile.addValue("minor", 6);
            settingsRecordARMFile.popTag();
            //end version 
            
            //recordARM header
            settingsRecordARMFile.addTag("RECORD");
            settingsRecordARMFile.pushTag("RECORD", numAboutTags-1);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            settingsRecordARMFile.addValue("DATE", date);
            settingsRecordARMFile.addValue("SESSION", recordSession);
            settingsRecordARMFile.addValue("PARTICIPANT", recordParticipant);
            settingsRecordARMFile.addValue("AGE", recordAge);
            settingsRecordARMFile.addValue("AMPUTATION", recordAmputation);
            settingsRecordARMFile.addValue("AUDIO", recordAudio);
            
            settingsRecordARMFile.addValue("CAMERA", -1);
            settingsRecordARMFile.addValue("PAIN_SCORE", -1);
            settingsRecordARMFile.addValue("TIME", "-1");
            settingsRecordARMFile.addValue("POS_X", -1);
            settingsRecordARMFile.addValue("POS_Y", -1);
            settingsRecordARMFile.addValue("POS_Z", -1);
            settingsRecordARMFile.addValue("XAXIS_X", -1);
            settingsRecordARMFile.addValue("XAXIS_Y", -1);
            settingsRecordARMFile.addValue("XAXIS_Z", -1);
            settingsRecordARMFile.addValue("YAXIS_X", -1);
            settingsRecordARMFile.addValue("YAXIS_Y", -1);
            settingsRecordARMFile.addValue("YAXIS_Z", -1);
            settingsRecordARMFile.addValue("ZAXIS_X", -1);
            settingsRecordARMFile.addValue("ZAXIS_Y", -1);
            settingsRecordARMFile.addValue("ZAXIS_Z", -1);
            settingsRecordARMFile.addValue("ENDRECORD", -1);
            
            settingsRecordARMFile.popTag();
            settingsRecordARMFile.pushTag("RECORD", numAboutTags-1);
            
            settingsRecordARMFile.popTag();
        }        
        settingsRecordARMFile.saveFile(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        clearDoc = false;
    }
    
    if (clearTagCon == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            settingsRecordARMFile.clearTagContents(whoClearTagCon, 0);
            settingsRecordARMFile.addValue(whoClearTagCon, whichTagCon);
            settingsRecordARMFile.popTag();
            
            settingsRecordARMFile.saveFile(currentXmlFile);
            message = currentXmlFile;
            message += " saved to xml!";
        }
        clearTagCon = false;
    }
    
    if (isRemoveTag == true) {
        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
            settingsRecordARMFile.removeTag(whoClearTagCon, 0);
            settingsRecordARMFile.addValue(whoClearTagCon, whichTagCon);
            settingsRecordARMFile.popTag();
            settingsRecordARMFile.saveFile(currentXmlFile);
            message = currentXmlFile;
            message += " saved to xml!";
        }
        isRemoveTag = false;
    }
    
    //-------------edit---------------_
}

//--------------------------------------------------------------
void recRecordARM::updateAddress(float setRecordAddress){
	recordAddress = setRecordAddress;
}

//--------------------------------------------------------------
void recRecordARM::updateClearTagContents(string newWhoClearTagCon, string newWhichTagCon){
    whoClearTagCon = newWhoClearTagCon;
    whichTagCon = newWhichTagCon;
}

//--------------------------------------------------------------
void recRecordARM::setRGBA(int setR, int setG, int setB, int setA){
    r = setR;
    g = setG;
    b = setB;
    a = setA;
}

//--------------------------------------------------------------
void recRecordARM::updateInfo(string setSession, string setParticipant, string setAge, string setAmputation, string setAudio){
    recordSession = setSession;
    recordParticipant = setParticipant;
    recordAge = setAge;
    recordAmputation = setAmputation;
    recordAudio = setAudio;
}
//--------------------------------------------------------------
void recRecordARM::draw(int setX, int setY, int setW, int setH){
}

//--------------------------------------------------------------
void recRecordARM::keyPressed  (int key){
    if (isLoggingKey == false) {
        //get-----------------_
        if(key == OF_KEY_UP || key == 357){
            --setRecordAddress;
            if (setRecordAddress <= 0) {
                setRecordAddress = 0;
            }
            isGetAll = true;
            if (dBug == true)cout << "setRecordAddress " << setRecordAddress << endl;
        }
        if(key == OF_KEY_DOWN || key == 359){
            ++setRecordAddress;
            if (setRecordAddress >= settingsRecordARMFile.getNumTags("RECORD")) {
                setRecordAddress = settingsRecordARMFile.getNumTags("RECORD")-1;
            }
            isGetAll = true;
            if (dBug == true)cout << "setRecordAddress " << setRecordAddress << endl;
        }
        if(key == OF_KEY_RIGHT || key == 258){
            ++setGetRecordAddressAll;
            if (setGetRecordAddressAll >= allCamera.size()) {
                setGetRecordAddressAll = allCamera.size()-1;
            }
            if (dBug == true)cout << " setGetRecordAddressAll                        " << setGetRecordAddressAll << endl;
        }
        if(key == OF_KEY_LEFT || key == 256){
            --setGetRecordAddressAll;
            if (setGetRecordAddressAll <= 0) {
                setGetRecordAddressAll = 0;
            }
            if (dBug == true)cout << " setGetRecordAddressAll " << setGetRecordAddressAll << endl;
        }
        //---------get--------_
        
        
        //edit----------------------------_
        if (dBug == true) {
            if(key == '0')clearDoc = true;
        
            if(key == 'S'){
                isSaveAll = true;
                settingsRecordARMFile.saveFile(currentXmlFile);
                message = currentXmlFile;
                message += " saved to xml!";
            }
            if(key == 's'){
                isSaveEvents = true;
            }
        }
        
        if(key == 'E'){
            if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
                settingsRecordARMFile.removeTag("ENDRECORD", 0);
                settingsRecordARMFile.addValue("ENDRECORD", recordAddress);
                settingsRecordARMFile.popTag();
            }
            settingsRecordARMFile.saveFile(currentXmlFile);
            message = currentXmlFile;
            message += " saved to xml!";
        }
        if(key == 'R'){
            lastRecTagNumber = settingsRecordARMFile.getNumTags("RECORD");
            if( settingsRecordARMFile.pushTag("RECORD", lastRecTagNumber-1) ){
                settingsRecordARMFile.removeTag("ENDRECORD", 0);
                settingsRecordARMFile.addValue("ENDRECORD", recordAddress);
                settingsRecordARMFile.popTag();
            }
            
            lastRecTagNumber	= settingsRecordARMFile.addTag("RECORD");
            xmlStructure	= "<RECORD>\n";
            
            if( settingsRecordARMFile.pushTag("RECORD", lastRecTagNumber) ){
                date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
                settingsRecordARMFile.addValue("DATE", date);
                settingsRecordARMFile.addValue("SESSION", recordSession);
                settingsRecordARMFile.addValue("PARTICIPANT", recordParticipant);
                settingsRecordARMFile.addValue("AGE", recordAge);
                settingsRecordARMFile.addValue("AMPUTATION", recordAmputation);
                settingsRecordARMFile.addValue("AUDIO", recordAudio);
                settingsRecordARMFile.popTag();
                settingsRecordARMFile.popTag();
            }
            int myCamera;
            myCamera = recordCamera;
            camera.push_back(myCamera);
            int myPainScore;
            myPainScore = recordPainScore;
            painScore.push_back(myPainScore);
            string myTime;
            myTime = recordTime;
            time.push_back(recordTime);
            
            float myPosition_X;
            myPosition_X = recordPosition_X;
            position_X.push_back(myPosition_X);
            float myPosition_Y;
            myPosition_Y = recordPosition_Y;
            position_Y.push_back(myPosition_Y);
            float myPosition_Z;
            myPosition_Z = recordPosition_Z;
            position_Z.push_back(myPosition_Z);
            
            float myXAxis_X;
            myXAxis_X = recordXAxis_X;
            xAxis_X.push_back(myXAxis_X);
            float myXAxis_Y;
            myXAxis_Y = recordXAxis_Y;
            xAxis_Y.push_back(myXAxis_Y);
            float myXAxis_Z;
            myXAxis_Z = recordXAxis_Z;
            xAxis_Z.push_back(myXAxis_Z);
            
            float myYAxis_X;
            myYAxis_X = recordYAxis_X;
            yAxis_X.push_back(myYAxis_X);
            float myYAxis_Y;
            myYAxis_Y = recordYAxis_Y;
            yAxis_Y.push_back(myYAxis_Y);
            float myYAxis_Z;
            myYAxis_Z = recordYAxis_Z;
            yAxis_Z.push_back(myYAxis_Z);
            
            float myZAxis_X;
            myZAxis_X = recordZAxis_X;
            zAxis_X.push_back(myZAxis_X);
            float myZAxis_Y;
            myZAxis_Y = recordZAxis_Y;
            zAxis_Y.push_back(myZAxis_Y);
            float myZAxis_Z;
            myZAxis_Z = recordZAxis_Z;
            zAxis_Z.push_back(myZAxis_Z);
            
            isStartEvents = true;
            isEventsTest = false;
            lastRecTagNumber = settingsRecordARMFile.getNumTags("RECORD");
            if( settingsRecordARMFile.pushTag("RECORD", lastRecTagNumber-1) ){
                settingsRecordARMFile.addValue("CAMERA", -1);
                settingsRecordARMFile.addValue("PAIN_SCORE", -1);
                settingsRecordARMFile.addValue("TIME", "-1");
                settingsRecordARMFile.addValue("POS_X", -1);
                settingsRecordARMFile.addValue("POS_Y", -1);
                settingsRecordARMFile.addValue("POS_Z", -1);
                
                settingsRecordARMFile.addValue("XAXIS_X", -1);
                settingsRecordARMFile.addValue("XAXIS_Y", -1);
                settingsRecordARMFile.addValue("XAXIS_Z", -1);
                
                settingsRecordARMFile.addValue("YAXIS_X", -1);
                settingsRecordARMFile.addValue("YAXIS_Y", -1);
                settingsRecordARMFile.addValue("YAXIS_Z", -1);

                settingsRecordARMFile.addValue("ZAXIS_X", -1);
                settingsRecordARMFile.addValue("ZAXIS_Y", -1);
                settingsRecordARMFile.addValue("ZAXIS_Z", -1);

                settingsRecordARMFile.addValue("ENDRECORD", -1);
                settingsRecordARMFile.popTag();
            }
        }
    }

    if (isLoggingKey == true) {
        if(key == '1'||
           key == '2'||
           key == '3'||
           key == '4'||
           key == '5'||
           key == '6'||
           key == '7'||
           key == '8'||
           key == '9'){
            int myCamera;
            myCamera = recordCamera;
            camera.push_back(myCamera);
            
            int myPainScore;
            recordPainScore = (key + 2)-50;//this to make the key to 1-9 and not 49-50-51...
            myPainScore = recordPainScore;
            painScore.push_back(myPainScore);
            
            string myTime;
            myTime = recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(ofGetElapsedTimeMillis());
            time.push_back(myTime);
            
            float myPosition_X;
            myPosition_X = recordPosition_X;
            position_X.push_back(myPosition_X);
            float myPosition_Y;
            myPosition_Y = recordPosition_Y;
            position_Y.push_back(myPosition_Y);
            float myPosition_Z;
            myPosition_Z = recordPosition_Z;
            position_Z.push_back(myPosition_Z);
            
            float myXAxis_X;
            myXAxis_X = recordXAxis_X;
            xAxis_X.push_back(myXAxis_X);
            float myXAxis_Y;
            myXAxis_Y = recordXAxis_Y;
            xAxis_Y.push_back(myXAxis_Y);
            float myXAxis_Z;
            myXAxis_Z = recordXAxis_Z;
            xAxis_Z.push_back(myXAxis_Z);
            
            float myYAxis_X;
            myYAxis_X = recordYAxis_X;
            yAxis_X.push_back(myYAxis_X);
            float myYAxis_Y;
            myYAxis_Y = recordYAxis_Y;
            yAxis_Y.push_back(myYAxis_Y);
            float myYAxis_Z;
            myYAxis_Z = recordYAxis_Z;
            yAxis_Z.push_back(myYAxis_Z);
            
            float myZAxis_X;
            myZAxis_X = recordZAxis_X;
            zAxis_X.push_back(myZAxis_X);
            float myZAxis_Y;
            myZAxis_Y = recordZAxis_Y;
            zAxis_Y.push_back(myZAxis_Y);
            float myZAxis_Z;
            myZAxis_Z = recordZAxis_Z;
            zAxis_Z.push_back(myZAxis_Z);
            
            isStartEvents = true;
            isEventsTest = false;
            isSaveAll = true;
        }
    }
    //------------edit----------------_
}