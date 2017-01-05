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
    
    isNewXmlFile = false;
    xmlFileTest = xmlFile;
    currentXmlFile = "record/address/flythrough/";
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
    
    if( settingsRecordARMFile.load(currentXmlFile) ){
        message = currentXmlFile + ".xml loaded!";
        
        message = currentXmlFile;
		message += " loaded!";
        cout << message << endl;
    }else{
        settingsRecordARMFile.addChild("FLYTHROUGH");
        settingsRecordARMFile.setTo("FLYTHROUGH");
        cout << "unable to load " << endl;
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
    
    setRecordAddressTest = setRecordAddress = 0;
    setCameraAddress = 0;
    //get----------------------------------_
    isGetAll = true;
    if (isGetAll == true) {
        if(settingsRecordARMFile.exists("about")){
            int numAboutTags = settingsRecordARMFile.getNumChildren("about");
        }else{
            //client about
            ofXml aboutXml;
            aboutXml.addChild("about");
            aboutXml.setTo("about");
            aboutXml.setAttribute("id", ofToString(0));
            
            aboutXml.addValue("file", currentXmlFile);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            aboutXml.addValue("DATE", date);
            aboutXml.addValue("client","--AugmentedRealityMirror-- Version: BETA.0");
            aboutXml.addValue("authors", "Lee Meredith");
            aboutXml.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
            aboutXml.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
            settingsRecordARMFile.addXml(aboutXml);
            //client end about
        }
        
        if(settingsRecordARMFile.exists("version")){
            int numVersTags = settingsRecordARMFile.getNumChildren("version");
        }else{
            //version
            ofXml versionXml;
            versionXml.addChild("version");
            versionXml.setTo("version");
            versionXml.setAttribute("id", ofToString(0));
            
            versionXml.addValue("major", 4);
            versionXml.addValue("minor", 0);
            settingsRecordARMFile.addXml(versionXml);
            //end version
        }
        
        if(settingsRecordARMFile.exists("RECORD")){
            int numRecTags = settingsRecordARMFile.getNumChildren("RECORD");
            
            settingsRecordARMFile.setTo("RECORD");
            
            getDate = settingsRecordARMFile.getValue<string>("DATE");
            getSession = settingsRecordARMFile.getValue<string>("SESSION");
            getParticipant = settingsRecordARMFile.getValue<string>("PARTICIPANT");
            getAge = settingsRecordARMFile.getValue<string>("AGE");
            getAmputation = settingsRecordARMFile.getValue<string>("AMPUTATION");
            getAudio = settingsRecordARMFile.getValue<string>("AUDIO");
            
            int numberOfSavedCamera = settingsRecordARMFile.getNumChildren("CAMERA");
            for (int i = 0; i < numberOfSavedCamera; i++) {
                int myCamera;
                myCamera = settingsRecordARMFile.getValue<int>("CAMERA[" + ofToString(i) + "]");// or ("X", 0)
                allCamera.push_back(myCamera);
            }
            int numberOfSavedPainScore = settingsRecordARMFile.getNumChildren("PAIN_SCORE");
            for (int i = 0; i < numberOfSavedPainScore; i++) {
                int myPainScore;
                myPainScore = settingsRecordARMFile.getValue<int>("PAIN_SCORE[" + ofToString(i) + "]");// or ("X", 0)
                allPainScores.push_back(myPainScore);
            }
            int numberOfSavedTime = settingsRecordARMFile.getNumChildren(("TIME"));
            for (int i = 0; i < numberOfSavedTime; i++) {
                string myTime;
                myTime = settingsRecordARMFile.getValue<string>("TIME[" + ofToString(i) + "]");// or ("X", 0)
                allTimes.push_back(myTime);
            }
            //position-------------------------_
            int numberOfSavedPosition_X = settingsRecordARMFile.getNumChildren("POS_X");
            for (int i = 0; i < numberOfSavedPosition_X; i++) {
                float myPosition_X;
                myPosition_X = settingsRecordARMFile.getValue<float>("POS_X[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_X.push_back(myPosition_X);
            }
            int numberOfSavedPosition_Y = settingsRecordARMFile.getNumChildren("POS_Y");
            for (int i = 0; i < numberOfSavedPosition_Y; i++) {
                float myPosition_Y;
                myPosition_Y = settingsRecordARMFile.getValue<float>("POS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_Y.push_back(myPosition_Y);
            }
            int numberOfSavedPosition_Z = settingsRecordARMFile.getNumChildren("POS_Z");
            for (int i = 0; i < numberOfSavedPosition_Z; i++) {
                float myPosition_Z;
                myPosition_Z = settingsRecordARMFile.getValue<float>("POS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_Z.push_back(myPosition_Z);
            }
            //-------------position------------_
            
            //XAxis-------------------------_
            int numberOfSavedXAxis_X = settingsRecordARMFile.getNumChildren("XAXIS_X");
            for (int i = 0; i < numberOfSavedXAxis_X; i++) {
                float myXAxis_X;
                myXAxis_X = settingsRecordARMFile.getValue<float>("XAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_X.push_back(myXAxis_X);
            }
            int numberOfSavedXAxis_Y = settingsRecordARMFile.getNumChildren("XAXIS_Y");
            for (int i = 0; i < numberOfSavedXAxis_Y; i++) {
                float myXAxis_Y;
                myXAxis_Y = settingsRecordARMFile.getValue<float>("XAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_Y.push_back(myXAxis_Y);
            }
            int numberOfSavedXAxis_Z = settingsRecordARMFile.getNumChildren("XAXIS_Z");
            for (int i = 0; i < numberOfSavedXAxis_Z; i++) {
                float myXAxis_Z;
                myXAxis_Z = settingsRecordARMFile.getValue<float>("XAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_Z.push_back(myXAxis_Z);
            }
            //------------XAxis-------------_
            //YAxis-------------------------_
            int numberOfSavedYAxis_X = settingsRecordARMFile.getNumChildren("YAXIS_X");
            for (int i = 0; i < numberOfSavedYAxis_X; i++) {
                float myYAxis_X;
                myYAxis_X = settingsRecordARMFile.getValue<float>("YAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_X.push_back(myYAxis_X);
            }
            int numberOfSavedYAxis_Y = settingsRecordARMFile.getNumChildren("YAXIS_Y");
            for (int i = 0; i < numberOfSavedYAxis_Y; i++) {
                float myYAxis_Y;
                myYAxis_Y = settingsRecordARMFile.getValue<float>("YAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_Y.push_back(myYAxis_Y);
            }
            int numberOfSavedYAxis_Z = settingsRecordARMFile.getNumChildren("YAXIS_Z");
            for (int i = 0; i < numberOfSavedYAxis_Z; i++) {
                float myYAxis_Z;
                myYAxis_Z = settingsRecordARMFile.getValue<float>("YAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_Z.push_back(myYAxis_Z);
            }
            //-----------YAxis--------------_
            //ZAxis-------------------------_
            int numberOfSavedZAxis_X = settingsRecordARMFile.getNumChildren("ZAXIS_X");
            for (int i = 0; i < numberOfSavedZAxis_X; i++) {
                float myZAxis_X;
                myZAxis_X = settingsRecordARMFile.getValue<float>("ZAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_X.push_back(myZAxis_X);
            }
            int numberOfSavedZAxis_Y = settingsRecordARMFile.getNumChildren("ZAXIS_Y");
            for (int i = 0; i < numberOfSavedZAxis_Y; i++) {
                float myZAxis_Y;
                myZAxis_Y = settingsRecordARMFile.getValue<float>("ZAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_Y.push_back(myZAxis_Y);
            }
            int numberOfSavedZAxis_Z = settingsRecordARMFile.getNumChildren("ZAXIS_Z");
            for (int i = 0; i < numberOfSavedZAxis_Z; i++) {
                float myZAxis_Z;
                myZAxis_Z = settingsRecordARMFile.getValue<float>("ZAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_Z.push_back(myZAxis_Z);
            }
            //-------------ZAxis------------_
            int numberOfSavedEndRec = settingsRecordARMFile.getNumChildren("ENDRECORD");
            if (numberOfSavedEndRec > 0) {
                lastTagNumberEndRec = settingsRecordARMFile.getValue<float>("ENDRECORD[" + ofToString(0) + "]");// or ("X", 0)
            }
            settingsRecordARMFile.setToParent();
        }else{
            //recordARM header
            ofXml recXml;
            recXml.addChild("RECORD");
            recXml.setTo("RECORD");
            recXml.setAttribute("id", ofToString(0));
            
            recXml.addValue("file", currentXmlFile);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            recXml.addValue("DATE", date);
            recXml.addValue("SESSION", recordSession);
            recXml.addValue("PARTICIPANT", recordParticipant);
            recXml.addValue("AGE", recordAge);
            recXml.addValue("AMPUTATION", recordAmputation);
            recXml.addValue("AUDIO", recordAudio);
            
            recXml.addValue("CAMERA", recordCamera);
            recXml.addValue("PAIN_SCORE", recordPainScore);
            int getElapsedTimeMillis = ofGetElapsedTimeMillis();
            getElapsedTimeMillis %= 60;
            recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(getElapsedTimeMillis);
            
            recordPosition_X = -15.9951;
            recordPosition_Y = 80.1833;
            recordPosition_Z = 916.36;
            
            recordXAxis_X = 0.999848l;
            recordXAxis_Y = -1.48748;
            recordXAxis_Z = 0.0174524;
           
            recordYAxis_X = 0.00152108;
            recordYAxis_Y = 0.996195;
            recordYAxis_Z = -0.0871425;
            
            recordZAxis_X = -0.017386;
            recordZAxis_Y = 0.0871557;
            recordZAxis_Z = 0.996043;
            
            recXml.addValue("TIME", recordTime);
            recXml.addValue("POS_X", recordPosition_X);
            recXml.addValue("POS_Y", recordPosition_Y);
            recXml.addValue("POS_Z", recordPosition_Z);
            
            recXml.addValue("XAXIS_X", recordXAxis_X);
            recXml.addValue("XAXIS_Y", recordXAxis_Y);
            recXml.addValue("XAXIS_Z", recordXAxis_Z);
            
            recXml.addValue("YAXIS_X", recordYAxis_X);
            recXml.addValue("YAXIS_Y", recordYAxis_Y);
            recXml.addValue("YAXIS_Z", recordYAxis_Z);
            
            recXml.addValue("ZAXIS_X", recordZAxis_X);
            recXml.addValue("ZAXIS_y", recordZAxis_Y);
            recXml.addValue("ZAXIS_Z", recordZAxis_Z);
            recXml.addValue("ENDRECORD", -1);
            settingsRecordARMFile.addXml(recXml);
            
            allCamera.push_back(recordCamera);
            allPainScores.push_back(recordPainScore);
            allTimes.push_back(recordTime);
            allPositions_X.push_back(recordPosition_X);
            allPositions_Y.push_back(recordPosition_Y);
            allPositions_Z.push_back(recordPosition_Z);
            
            allXAxis_X.push_back(recordXAxis_X);
            allXAxis_Y.push_back(recordXAxis_Y);
            allXAxis_Z.push_back(recordXAxis_Z);
            
            allYAxis_X.push_back(recordYAxis_X);
            allYAxis_Y.push_back(recordYAxis_Y);
            allYAxis_Z.push_back(recordYAxis_Z);
            
            allZAxis_X.push_back(recordZAxis_X);
            allZAxis_Y.push_back(recordZAxis_Y);
            allZAxis_Z.push_back(recordZAxis_Z);
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
    settingsRecordARMFile.save(currentXmlFile);
    message = currentXmlFile;
    message += " saved to xml!";
    isSaveAll = false;
    //---------------get-------------------_
    
    //edit----------------------------_
    isSaveAll = isSaveEvents = isStartEvents = isSetRecord = false;
    isLogging = isLoggingKey = false;
    //------------edit----------------_
}

//--------------------------------------------------------------
void recRecordARM::update(string xmlFile){
    
    if (xmlFile != xmlFileTest) {
        currentXmlFile = "record/address/flythrough/";
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
        
        if( settingsRecordARMFile.load(currentXmlFile) ){
            message = currentXmlFile;
            message += " loaded!";
        }else{
           
            message = "unable to load ";
            message += currentXmlFile;
            
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            whoClearTagCon = "SESSION";
            whichTagCon = "tesuji";
            recordAddress = 0.1;
            recordSession = "0";
            recordParticipant = "who";
            recordAge = "120";
            recordAmputation = "BK";
            recordAudio = "heyYou.wav";
        }
        //settingsRecordARMFile.load(currentXmlFile);
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
        
        if( settingsRecordARMFile.load(currentXmlFile) ){
            message = currentXmlFile + ".xml loaded!";
            isNewXmlFile = false;
            message = currentXmlFile;
            message += " loaded!";
            cout << message << endl;
        }else{
            cout << "unable to load " << endl;
            message = "unable to load ";
            message += currentXmlFile;
            cout << message << endl;
            isNewXmlFile = true;
        }
        
        if (setRecordAddress != setRecordAddressTest) {
            setRecordAddressTest = setRecordAddress;
            isSetRecordAddressNew = true;
        }
        if (setRecordAddress == setRecordAddressTest){
            isSetRecordAddressNew = false;
        }
        
        if(settingsRecordARMFile.exists("FLYTHROUGH")){
            cout << " settingsRecordARMFile.exists(FLYTHROUGH) " << endl;
        }else {
            if (isNewXmlFile == true) {
                settingsRecordARMFile.addChild("FLYTHROUGH");
            }
        }
        settingsRecordARMFile.setTo("FLYTHROUGH");
        
        if(settingsRecordARMFile.exists("about")){
            int numAboutTags = settingsRecordARMFile.getNumChildren("about");
        }else{
            //client about
            ofXml aboutXml;
            aboutXml.addChild("about");
            aboutXml.setTo("about");
            aboutXml.setAttribute("id", ofToString(0));
            
            aboutXml.addValue("file", currentXmlFile);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            aboutXml.addValue("DATE", date);
            aboutXml.addValue("client","--AugmentedRealityMirror-- Version: BETA.0");
            aboutXml.addValue("authors", "Lee Meredith");
            aboutXml.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
            aboutXml.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
            settingsRecordARMFile.addXml(aboutXml);
            //client end about
        }
        
        if(settingsRecordARMFile.exists("version")){
            int numVersTags = settingsRecordARMFile.getNumChildren("version");
        }else{
            //version
            ofXml versionXml;
            versionXml.addChild("version");
            versionXml.setTo("version");
            versionXml.setAttribute("id", ofToString(0));
            
            versionXml.addValue("major", 4);
            versionXml.addValue("minor", 0);
            settingsRecordARMFile.addXml(versionXml);
            //end version
        }

        if(settingsRecordARMFile.exists("RECORD")){
            int numRecTags = settingsRecordARMFile.getNumChildren("RECORD");
            
            settingsRecordARMFile.setTo("RECORD");
            
            getDate = settingsRecordARMFile.getValue<string>("DATE");
            getSession = settingsRecordARMFile.getValue<string>("SESSION");
            getParticipant = settingsRecordARMFile.getValue<string>("PARTICIPANT");
            getAge = settingsRecordARMFile.getValue<string>("AGE");
            getAmputation = settingsRecordARMFile.getValue<string>("AMPUTATION");
            getAudio = settingsRecordARMFile.getValue<string>("AUDIO");
            
            int numberOfSavedCamera = settingsRecordARMFile.getNumChildren("CAMERA");
            for (int i = 0; i < numberOfSavedCamera; i++) {
                int myCamera;
                myCamera = settingsRecordARMFile.getValue<int>("CAMERA[" + ofToString(i) + "]");// or ("X", 0)
                allCamera.push_back(myCamera);
            }
            int numberOfSavedPainScore = settingsRecordARMFile.getNumChildren("PAIN_SCORE");
            for (int i = 0; i < numberOfSavedPainScore; i++) {
                int myPainScore;
                myPainScore = settingsRecordARMFile.getValue<int>("PAIN_SCORE[" + ofToString(i) + "]");// or ("X", 0)
                allPainScores.push_back(myPainScore);
            }
            int numberOfSavedTime = settingsRecordARMFile.getNumChildren(("TIME"));
            for (int i = 0; i < numberOfSavedTime; i++) {
                string myTime;
                myTime = settingsRecordARMFile.getValue<string>("TIME[" + ofToString(i) + "]");// or ("X", 0)
                allTimes.push_back(myTime);
            }
            //position-------------------------_
            int numberOfSavedPosition_X = settingsRecordARMFile.getNumChildren("POS_X");
            for (int i = 0; i < numberOfSavedPosition_X; i++) {
                float myPosition_X;
                myPosition_X = settingsRecordARMFile.getValue<float>("POS_X[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_X.push_back(myPosition_X);
                
            }
            int numberOfSavedPosition_Y = settingsRecordARMFile.getNumChildren("POS_Y");
            for (int i = 0; i < numberOfSavedPosition_Y; i++) {
                float myPosition_Y;
                myPosition_Y = settingsRecordARMFile.getValue<float>("POS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_Y.push_back(myPosition_Y);
            }
            int numberOfSavedPosition_Z = settingsRecordARMFile.getNumChildren("POS_Z");
            for (int i = 0; i < numberOfSavedPosition_Z; i++) {
                float myPosition_Z;
                myPosition_Z = settingsRecordARMFile.getValue<float>("POS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allPositions_Z.push_back(myPosition_Z);
            }
            //-------------position------------_
            
            //XAxis-------------------------_
            int numberOfSavedXAxis_X = settingsRecordARMFile.getNumChildren("XAXIS_X");
            for (int i = 0; i < numberOfSavedXAxis_X; i++) {
                float myXAxis_X;
                myXAxis_X = settingsRecordARMFile.getValue<float>("XAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_X.push_back(myXAxis_X);
            }
            int numberOfSavedXAxis_Y = settingsRecordARMFile.getNumChildren("XAXIS_Y");
            for (int i = 0; i < numberOfSavedXAxis_Y; i++) {
                float myXAxis_Y;
                myXAxis_Y = settingsRecordARMFile.getValue<float>("XAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_Y.push_back(myXAxis_Y);
            }
            int numberOfSavedXAxis_Z = settingsRecordARMFile.getNumChildren("XAXIS_Z");
            for (int i = 0; i < numberOfSavedXAxis_Z; i++) {
                float myXAxis_Z;
                myXAxis_Z = settingsRecordARMFile.getValue<float>("XAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allXAxis_Z.push_back(myXAxis_Z);
            }
            //------------XAxis-------------_
            //YAxis-------------------------_
            int numberOfSavedYAxis_X = settingsRecordARMFile.getNumChildren("YAXIS_X");
            for (int i = 0; i < numberOfSavedYAxis_X; i++) {
                float myYAxis_X;
                myYAxis_X = settingsRecordARMFile.getValue<float>("YAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_X.push_back(myYAxis_X);
            }
            int numberOfSavedYAxis_Y = settingsRecordARMFile.getNumChildren("YAXIS_Y");
            for (int i = 0; i < numberOfSavedYAxis_Y; i++) {
                float myYAxis_Y;
                myYAxis_Y = settingsRecordARMFile.getValue<float>("YAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_Y.push_back(myYAxis_Y);
            }
            int numberOfSavedYAxis_Z = settingsRecordARMFile.getNumChildren("YAXIS_Z");
            for (int i = 0; i < numberOfSavedYAxis_Z; i++) {
                float myYAxis_Z;
                myYAxis_Z = settingsRecordARMFile.getValue<float>("YAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allYAxis_Z.push_back(myYAxis_Z);
            }
            //-----------YAxis--------------_
            //ZAxis-------------------------_
            int numberOfSavedZAxis_X = settingsRecordARMFile.getNumChildren("ZAXIS_X");
            for (int i = 0; i < numberOfSavedZAxis_X; i++) {
                float myZAxis_X;
                myZAxis_X = settingsRecordARMFile.getValue<float>("ZAXIS_X[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_X.push_back(myZAxis_X);
            }
            int numberOfSavedZAxis_Y = settingsRecordARMFile.getNumChildren("ZAXIS_Y");
            for (int i = 0; i < numberOfSavedZAxis_Y; i++) {
                float myZAxis_Y;
                myZAxis_Y = settingsRecordARMFile.getValue<float>("ZAXIS_Y[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_Y.push_back(myZAxis_Y);
            }
            int numberOfSavedZAxis_Z = settingsRecordARMFile.getNumChildren("ZAXIS_Z");
            for (int i = 0; i < numberOfSavedZAxis_Z; i++) {
                float myZAxis_Z;
                myZAxis_Z = settingsRecordARMFile.getValue<float>("ZAXIS_Z[" + ofToString(i) + "]");// or ("X", 0)
                allZAxis_Z.push_back(myZAxis_Z);
            }
            //-------------ZAxis------------_
            int numberOfSavedEndRec = settingsRecordARMFile.getNumChildren("ENDRECORD");
            if (numberOfSavedEndRec > 0) {
                lastTagNumberEndRec = settingsRecordARMFile.getValue<float>("ENDRECORD[" + ofToString(0) + "]");// or ("X", 0)
            }
            settingsRecordARMFile.setToParent();
        }else{
            //recordARM header
            ofXml recXml;
            recXml.addChild("RECORD");
            recXml.setTo("RECORD");
            recXml.setAttribute("id", ofToString(0));
            
            recXml.addValue("file", currentXmlFile);
            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
            recXml.addValue("DATE", date);
            recXml.addValue("SESSION", recordSession);
            recXml.addValue("PARTICIPANT", recordParticipant);
            recXml.addValue("AGE", recordAge);
            recXml.addValue("AMPUTATION", recordAmputation);
            recXml.addValue("AUDIO", recordAudio);
            
            recXml.addValue("CAMERA", recordCamera);
            recXml.addValue("PAIN_SCORE", recordPainScore);
            int getElapsedTimeMillis = ofGetElapsedTimeMillis();
            getElapsedTimeMillis %= 60;
            recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(getElapsedTimeMillis);
            recXml.addValue("TIME", recordTime);
            
            recordPosition_X = -15.9951;
            recordPosition_Y = 80.1833;
            recordPosition_Z = 916.36;
            
            recordXAxis_X = 0.999848l;
            recordXAxis_Y = -1.48748;
            recordXAxis_Z = 0.0174524;
            
            recordYAxis_X = 0.00152108;
            recordYAxis_Y = 0.996195;
            recordYAxis_Z = -0.0871425;
            
            recordZAxis_X = -0.017386;
            recordZAxis_Y = 0.0871557;
            recordZAxis_Z = 0.996043;
            
            recXml.addValue("POS_X", recordPosition_X);
            recXml.addValue("POS_Y", recordPosition_Y);
            recXml.addValue("POS_Z", recordPosition_Z);
            
            recXml.addValue("XAXIS_X", recordXAxis_X);
            recXml.addValue("XAXIS_Y", recordXAxis_Y);
            recXml.addValue("XAXIS_Z", recordXAxis_Z);
            
            recXml.addValue("YAXIS_X", recordYAxis_X);
            recXml.addValue("YAXIS_Y", recordYAxis_Y);
            recXml.addValue("YAXIS_Z", recordYAxis_Z);
            
            recXml.addValue("ZAXIS_X", recordZAxis_X);
            recXml.addValue("ZAXIS_y", recordZAxis_Y);
            recXml.addValue("ZAXIS_Z", recordZAxis_Z);
            
            recXml.addValue("ENDRECORD", -1);
            settingsRecordARMFile.addXml(recXml);
            
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
    isClearSave = false;
    if (isClearSave == true){
//        settingsRecordARMFile.clear();
//        settingsRecordARMFile.addChild("FLYTHROUGH");
//        settingsRecordARMFile.setTo("FLYTHROUGH");
        
//        isAtRun = false;
//        isTimerStart = true;
//        //isLoaded[currentDirXmlFile] = true;
//        isTimerOn = true;
        //client about
        ofXml aboutXml;
        aboutXml.addChild("about");
        aboutXml.setTo("about");
        aboutXml.setAttribute("id", ofToString(0));
        
        aboutXml.addValue("file", currentXmlFile);
        date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        aboutXml.addValue("DATE", date);
        aboutXml.addValue("client","--AugmentedRealityMirror-- Version: BETA.0");
        aboutXml.addValue("authors", "Lee Meredith");
        aboutXml.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
        aboutXml.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
        settingsRecordARMFile.addXml(aboutXml);
        //client end about
        
        //version
        ofXml versionXml;
        versionXml.addChild("version");
        versionXml.setTo("version");
        versionXml.setAttribute("id", ofToString(0));
        
        versionXml.addValue("major", 4);
        versionXml.addValue("minor", 0);
        settingsRecordARMFile.addXml(versionXml);
        //end version

        if (allCamera.size() > 0){
            for (int i=0; i < allCamera.size(); i++) {
                ofXml recXml;
                recXml.addChild("RECORD");
                recXml.setTo("RECORD");
                recXml.setAttribute("id", ofToString(0));
                recXml.addValue("CAMERA", allCamera[i]);
                recXml.addValue("PAIN_SCORE", allPainScores[i]);
                recXml.addValue("TIME", allTimes[i]);
                recXml.addValue("POS_X", allPositions_X[i]);
                recXml.addValue("POS_Y", allPositions_Y[i]);
                recXml.addValue("POS_Z", allPositions_Z[i]);
                recXml.addValue("XAXIS_X", allXAxis_X[i]);
                recXml.addValue("XAXIS_Y", allXAxis_Y[i]);
                recXml.addValue("XAXIS_Z", allXAxis_Z[i]);
                recXml.addValue("YAXIS_X", allYAxis_X[i]);
                recXml.addValue("YAXIS_Y", allYAxis_Y[i]);
                recXml.addValue("YAXIS_Z", allYAxis_Z[i]);
                recXml.addValue("ZAXIS_X", allZAxis_X[i]);
                recXml.addValue("ZAXIS_Y", allZAxis_Y[i]);
                recXml.addValue("ZAXIS_Z", allZAxis_Z[i]);
                settingsRecordARMFile.addXml(recXml);
            }
            isClearSave = false;
        }
    }
    
    if(isLogging == true){
        isStartEvents = true;
        isEventsTest = false;
        isSaveAll = true;
        isLogging = false;
    }
    
    if (isSaveAll == true) {
        settingsRecordARMFile.save(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        isSaveAll = false;
    }
//is this need or ???
//    if (isSetRecord == true) {
//        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
//            date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
//            settingsRecordARMFile.setValue("DATE", date);
//            settingsRecordARMFile.setValue("SESSION", recordSession);
//            settingsRecordARMFile.setValue("PARTICIPANT", recordParticipant);
//            settingsRecordARMFile.setValue("AGE", recordAge);
//            settingsRecordARMFile.setValue("AMPUTATION", recordAmputation);
//            settingsRecordARMFile.setValue("AUDIO", recordAudio);
//            settingsRecordARMFile.popTag();
//        }
//        isSetRecord = false;
//    }
    
    if (isSaveEvents == true) {
        //if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){}
        settingsRecordARMFile.setTo("RECORD");
        
        settingsRecordARMFile.addValue("CAMERA", recordCamera);
        recordPainScore = 0;
        settingsRecordARMFile.addValue("PAIN_SCORE", recordPainScore);
        int getElapsedTimeMillis = ofGetElapsedTimeMillis();
        getElapsedTimeMillis %= 60;
        recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(getElapsedTimeMillis);
        settingsRecordARMFile.addValue("TIME", recordTime);
        settingsRecordARMFile.addValue("POS_X", recordPosition_X);
        settingsRecordARMFile.addValue("POS_Y", recordPosition_Y);
        settingsRecordARMFile.addValue("POS_Z", recordPosition_Z);
        
        settingsRecordARMFile.addValue("XAXIS_X", recordXAxis_X);
        settingsRecordARMFile.addValue("XAXIS_Y", recordXAxis_Y);
        settingsRecordARMFile.addValue("XAXIS_Z", recordXAxis_Z);
        
        settingsRecordARMFile.addValue("YAXIS_X", recordYAxis_X);
        settingsRecordARMFile.addValue("YAXIS_Y", recordYAxis_Y);
        settingsRecordARMFile.addValue("YAXIS_Z", recordYAxis_Z);
        
        settingsRecordARMFile.addValue("ZAXIS_X", recordZAxis_X);
        settingsRecordARMFile.addValue("ZAXIS_y", recordZAxis_Y);
        settingsRecordARMFile.addValue("ZAXIS_Z", recordZAxis_Z);

        
        settingsRecordARMFile.save(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        isSaveEvents = false;
    }
    
    if (clearDoc == true) {
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
        
        if(settingsRecordARMFile.exists("FLYTHROUGH") == false){
            settingsRecordARMFile.addChild("FLYTHROUGH");
        }
        settingsRecordARMFile.setTo("FLYTHROUGH");
        //client about
        ofXml aboutXml;
        aboutXml.addChild("about");
        aboutXml.setTo("about");
        aboutXml.setAttribute("id", ofToString(0));
        
        aboutXml.addValue("file", currentXmlFile);
        date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        aboutXml.addValue("DATE", date);
        aboutXml.addValue("client","--AugmentedRealityMirror-- Version: BETA.0");
        aboutXml.addValue("authors", "Lee Meredith");
        aboutXml.addValue("url","https://github.com/leeMeredith/AugmentedRealityMirror");
        aboutXml.addValue("github", "https://github.com/leeMeredith/AugmentedRealityMirror");
        settingsRecordARMFile.addXml(aboutXml);
        //client end about
        
        //version
        ofXml versionXml;
        versionXml.addChild("version");
        versionXml.setTo("version");
        versionXml.setAttribute("id", ofToString(0));
        
        versionXml.addValue("major", 4);
        versionXml.addValue("minor", 0);
        settingsRecordARMFile.addXml(versionXml);
        //end version
        
        //recordARM header
        ofXml recXml;
        recXml.addChild("RECORD");
        recXml.setTo("RECORD");
        recXml.setAttribute("id", ofToString(0));
        
        recXml.addValue("file", currentXmlFile);
        date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        recXml.addValue("DATE", date);
        recXml.addValue("SESSION", recordSession);
        recXml.addValue("PARTICIPANT", recordParticipant);
        recXml.addValue("AGE", recordAge);
        recXml.addValue("AMPUTATION", recordAmputation);
        recXml.addValue("AUDIO", recordAudio);
        
        recXml.addValue("CAMERA", recordCamera);
        recXml.addValue("PAIN_SCORE", recordPainScore);
        int getElapsedTimeMillis = ofGetElapsedTimeMillis();
        getElapsedTimeMillis %= 60;
        recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(getElapsedTimeMillis);
        recXml.addValue("TIME", recordTime);
        recXml.addValue("POS_X", recordPosition_X);
        recXml.addValue("POS_Y", recordPosition_Y);
        recXml.addValue("POS_Z", recordPosition_Z);
        
        recXml.addValue("XAXIS_X", recordXAxis_X);
        recXml.addValue("XAXIS_Y", recordXAxis_Y);
        recXml.addValue("XAXIS_Z", recordXAxis_Z);
        
        recXml.addValue("YAXIS_X", recordYAxis_X);
        recXml.addValue("YAXIS_Y", recordYAxis_Y);
        recXml.addValue("YAXIS_Z", recordYAxis_Z);
        
        recXml.addValue("ZAXIS_X", recordZAxis_X);
        recXml.addValue("ZAXIS_y", recordZAxis_Y);
        recXml.addValue("ZAXIS_Z", recordZAxis_Z);
        recXml.addValue("ENDRECORD", -1);
        settingsRecordARMFile.addXml(recXml);
        
        settingsRecordARMFile.save(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        clearDoc = false;
    }
    
    if (clearTagCon == true) {
        settingsRecordARMFile.clear();
        settingsRecordARMFile.addChild("FLYTHROUGH");
        settingsRecordARMFile.setTo("FLYTHROUGH");
//        if( settingsRecordARMFile.pushTag("RECORD", setRecordAddress) ){
//            settingsRecordARMFile.clear(whoClearTagCon, 0);
//            settingsRecordARMFile.addValue(whoClearTagCon, whichTagCon);
//            settingsRecordARMFile.popTag();
//            
//            settingsRecordARMFile.saveFile(currentXmlFile);
//            message = currentXmlFile;
//            message += " saved to xml!";
//        }
        clearTagCon = false;
    }
    
    if (isRemoveTag == true) {
        lastRecTagNumber = settingsRecordARMFile.getNumChildren("RECORD");
        if(settingsRecordARMFile.exists("RECORD")){
            settingsRecordARMFile.setTo("RECORD");
            settingsRecordARMFile.setValue(whoClearTagCon, whichTagCon);
            cout << " whoClearTagCon " << whoClearTagCon << " whichTagCon " <<  whichTagCon << endl;
        }
 
        settingsRecordARMFile.save(currentXmlFile);
        message = currentXmlFile;
        message += " saved to xml!";
        
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
            if (setRecordAddress >= settingsRecordARMFile.getNumChildren("RECORD")) {
                setRecordAddress = settingsRecordARMFile.getNumChildren("RECORD")-1;
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
                settingsRecordARMFile.save(currentXmlFile);
                message = currentXmlFile;
                message += " saved to xml!";
            }
            if(key == 's'){
                isSaveEvents = true;
            }
        }
        
        if(key == 'E'){
            settingsRecordARMFile.remove("ENDRECORD[" + ofToString(0) + "]");
            settingsRecordARMFile.addValue("ENDRECORD", recordAddress);
            settingsRecordARMFile.save(currentXmlFile);
            message = currentXmlFile;
            message += " saved to xml!";
        }
        
        if(key == 'R'){
//            isStartEvents = true;
            isEventsTest = false;
            lastRecTagNumber = settingsRecordARMFile.getNumChildren("RECORD");
            if(settingsRecordARMFile.exists("RECORD")){
                settingsRecordARMFile.setTo("RECORD[" + ofToString(lastRecTagNumber-1) + "]");
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
            settingsRecordARMFile.setTo("RECORD");
            
            settingsRecordARMFile.addValue("CAMERA", recordCamera);
            recordPainScore = (key + 2)-50;//this to make the key to 1-9 and not 49-50-51...
            settingsRecordARMFile.addValue("PAIN_SCORE", recordPainScore);
            int getElapsedTimeMillis = ofGetElapsedTimeMillis();
            getElapsedTimeMillis %= 60;
            recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds())+"-"+ ofToString(getElapsedTimeMillis);
            settingsRecordARMFile.addValue("TIME", recordTime);
            settingsRecordARMFile.addValue("POS_X", recordPosition_X);
            settingsRecordARMFile.addValue("POS_Y", recordPosition_Y);
            settingsRecordARMFile.addValue("POS_Z", recordPosition_Z);
            
            settingsRecordARMFile.addValue("XAXIS_X", recordXAxis_X);
            settingsRecordARMFile.addValue("XAXIS_Y", recordXAxis_Y);
            settingsRecordARMFile.addValue("XAXIS_Z", recordXAxis_Z);
            
            settingsRecordARMFile.addValue("YAXIS_X", recordYAxis_X);
            settingsRecordARMFile.addValue("YAXIS_Y", recordYAxis_Y);
            settingsRecordARMFile.addValue("YAXIS_Z", recordYAxis_Z);
            
            settingsRecordARMFile.addValue("ZAXIS_X", recordZAxis_X);
            settingsRecordARMFile.addValue("ZAXIS_y", recordZAxis_Y);
            settingsRecordARMFile.addValue("ZAXIS_Z", recordZAxis_Z);
            
            //isStartEvents = true;
            isEventsTest = false;
            isSaveAll = true;
        }
    }
    //------------edit----------------_
}