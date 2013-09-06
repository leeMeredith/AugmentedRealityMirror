//
//  Created by Lee Meredith.
//

#include "dataLoggerARM.h"

//--------------------------------------------------------------
void dataLoggerARM::setup(){
    
    //ofDirectory------------------------_
    //Directory-Address-------_    
    recordAddressDir.listDir("record/address/");
	recordAddressDir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    if ((int)recordAddressDir.size() == 0) {
        setRecordFileDefault = setRecordFile = recordFile = "Demo";// if no file in record/address/????.xml
        recRecordARM_0.isSaveAll = true;
        recRecordARM_0.isGetAll = true;
    }
    isJustAtRunFile = true;
    recAddressCurrentDir = 0;
    isRecordAddressR = false;
    isRecordAddressL = false;
    isKeyFileRL = false;
    //---Directory-Address----_
    
    //Directory-Audio-------_
    recordAudioDir.listDir("record/media/audio/");
	recordAudioDir.sort();
    isKeyAudioRL = false;
    //----Directory-Audio---_
    
    //Directory-ImageFolder-------_
    /*
    dirARMImageFolderStringDefault = "record/media/images/";
    dirARMImageFolderString = dirARMImageFolderStringDefault;
    dirImageFolder.listDir(dirARMImageFolderString);
	dirImageFolder.sort();
    
    isJustAtRunImageFolder = true;
    recImageFolderCurrentDir = 0;
    isRecordImageFolderD = false;
    isRecordImageFolderUp = false;
    //----Directory-ImageFolder---_
    
    //Directory-Image-------_
    dirARMImageStringDefault = "record/media/images/";
    dirARMImageString = dirARMImageStringDefault;
    setDirirARMImageString = "bob";
    dirARMImageString += setDirirARMImageString;
    dirARMImageString += "/";
    dirARMImage.listDir(dirARMImageString);
	dirARMImage.sort();

	if( dirARMImage.size() ){
		images.assign(dirARMImage.size(), ofImage());
	}
    
	for(int i = 0; i < (int)dirARMImage.size(); i++){
		images[i].loadImage(dirARMImage.getPath(i));
	}
	currentARMImage = 0;
    setRecordImageFolderTest = dirARMImageString;
    //----Directory-Image---_
     */
    //-------------ofDirectory-----------_
    
    //audioPlayer--------------_
    nameOfDirPathAudio = "record/media/audio/loveHate.wav";
    audioPlayer_0.setup(20, 440, "record/media/audio/loveHate.wav");
    //-------audioPlayer-------_
    
	ofBackground(75,75,75);
    date = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
    isSaveAll = isSaveEvents = isStartEvents = isNothing = false;
    isEventsTest = true;
    clearDoc = clearTagCon = false;
    recordAddress = 0.1;
    isSetAllDefault = true;
    isSetRecordFileDefault = isSetAudioDefault = isSetARMImagesDefault = isSetARMImagesFolderDefault = false;
    isKeyFile = isKeySession = isKeyParticipant = isKeyAudio = isKeyRecAge = isKeyRecAmputation = false;//isKeyDiaImage
    recAddressCurrentDir = 0;
    for(int i = 0; i < (int)recordAddressDir.size(); i++){
		if(i == recAddressCurrentDir) {
            setRecordFileDefault = setRecordFile = recordFile = recordAddressDir.getName(i);
		}
	}
    
    setRecordSessionDefault = setRecordSession = recordSession = "1";
    setRecordParticipantDefault = setRecordParticipant = recordParticipant = "Who";
    setRecordAudioDefault = setRecordAudio = recordAudio = "heyYou.wav"; 
    setRecordAgeDefault = setRecordAge = recordAge = "130";
    setRecordAmputationDefault = setRecordAmputation = recordAmputation = "Level";
    //setRecordImageFolderDefault = setRecordImageFolder = recordImageFolder = "bob"; // did i need this?
    //setRecordImageDefault = setRecordImage = recordImage = "bob";
    
    setGetRecordAddress = setGetEventsAddress = setGetWordsAddress = setGetLettersAddress = setGetARMAddress = setGetNothingsAddress = 0;
    
    //recRecordARM------------------_
    graphRecARM_X = 10;
    graphRecARM_Y = 10;
    graphRecARM_W = 100;
    graphRecARM_H = 100;
    
    graphRecARM_R = 255;
    graphRecARM_G = 255;
    graphRecARM_B = 255;
    graphRecARM_A = 255;
    
    recRecordWhoClearTagCon = "Session";
    recRecordWhichTagCon = "cat";
    recRecordARM_0.setup(graphRecARM_X, graphRecARM_Y, recordFile);
    recRecordARM_0.updateAddress(audioPlayer_0.getLargePositionMS);
    recRecordARM_0.setRGBA(graphRecARM_R, graphRecARM_G, graphRecARM_B, graphRecARM_A);
    recRecordARM_0.updateInfo(recordSession, recordParticipant, recordAge, recordAmputation, recordAudio);
    recRecordWhoClearTagCon = guiMessXML_0.getMessRecordXMLTag;
    recRecordARM_0.updateClearTagContents(recRecordWhoClearTagCon, recRecordWhichTagCon);
    
    float setWithRan = ofRandom(0.9, 10.9);
    recCamera = recPainScore = 1;
    getAllPositionsXSize = 0;
    recPosition_X = recPosition_Y = recPosition_Z = setWithRan;
    recXAxis_X = recXAxis_Y = recXAxis_Z = setWithRan;
    recYAxis_X = recYAxis_Y = recYAxis_Z = setWithRan;
    recZAxis_X = recZAxis_Y = recZAxis_Z = setWithRan;
    recRecordARM_0.recordCamera = recCamera;
    recRecordARM_0.recordPainScore = recPainScore;
    recRecordARM_0.recordTime = "-1";
    
    recRecordARM_0.recordPosition_X = recPosition_X;
    recRecordARM_0.recordPosition_Y = recPosition_Y;
    recRecordARM_0.recordPosition_Z = recPosition_Z;
    
    recRecordARM_0.recordXAxis_X = recXAxis_X;
    recRecordARM_0.recordXAxis_Y = recXAxis_Y;
    recRecordARM_0.recordXAxis_Z = recXAxis_Z;
    
    recRecordARM_0.recordYAxis_X = recYAxis_X;
    recRecordARM_0.recordYAxis_Y = recYAxis_Y;
    recRecordARM_0.recordYAxis_Z = recYAxis_Z;
    
    recRecordARM_0.recordZAxis_X = recZAxis_X;
    recRecordARM_0.recordZAxis_Y = recZAxis_Y;
    recRecordARM_0.recordZAxis_Z = recZAxis_Z;    
    //--------recRecordARM----------_
        
    //guiMessXML------------------------_
    guiMessXML_0.setup(20, 150, "record/message/message_presets_ARM.xml");
    guiGetRecMessXML_0.setup(20, 290, "record/message/message_presets_ARM.xml");
    guiMessXML_0.recIndex_Y = guiMessXML_0.maxNumInList;
    //-----------guiMessXML-------------_
    
    //xenoRectOne--------------_
    getPositions_X = getPositions_Y = getPositions_Z = 0;
    setPosAddres = 0;
    //xenoRectOne--------------_
}

//--------------------------------------------------------------
void dataLoggerARM::update(){
    
    //ofDirectory------------------------_
    //Directory-Address-------_
    if (recordAddressDir.size() > 0 && isRecordAddressR == true){
		recAddressCurrentDir++;
		recAddressCurrentDir %= recordAddressDir.size();
        isRecordAddressR = false;
	}
    if (recordAddressDir.size() > 0 && isRecordAddressL == true){
		recAddressCurrentDir--;
        if (recAddressCurrentDir < 0) {
            recAddressCurrentDir = recordAddressDir.size()-1;
        }
        isRecordAddressL = false;
	}
    for(int i = 0; i < (int)recordAddressDir.size(); i++){
		if(i == recAddressCurrentDir) {
            if (isKeyFile == false && recAddressCurrentDirTest == false){
                setRecordFile = recordAddressDir.getName(i);
            }
		}
	}
    //---Directory-Address----_
    
    //Directory-Audio-------_
    if (recordAudioDir.size() > 0 && isRecordAudioR == true){
		recAudioCurrentDir++;
		recAudioCurrentDir %= recordAudioDir.size();
        isKeyAudioRL = true;
        isRecordAudioR = false;
	}
    if (recordAudioDir.size() > 0 && isRecordAudioL == true){
		recAudioCurrentDir--;
        if (recAudioCurrentDir < 0) {
            recAudioCurrentDir = recordAudioDir.size()-1;
        }
        isKeyAudioRL = true;
        isRecordAudioL = false;
	}
    for(int i = 0; i < (int)recordAudioDir.size(); i++){
		if(i == recAudioCurrentDir) {
            if (isKeyAudio == false && recAudioCurrentDirTest == false){
                setRecordAudio = recordAudioDir.getName(i);
            }
		}
	}
    //----Directory-Audio---_
    
    //Directory-ImageFolder-------_
    /*
    if (dirImageFolder.size() > 0 && isRecordImageFolderUp == true){
		recImageFolderCurrentDir++;
		recImageFolderCurrentDir %= dirImageFolder.size();
        isRecordImageFolderUp = false;
	}
    if (dirImageFolder.size() > 0 && isRecordImageFolderD == true){
		recImageFolderCurrentDir--;
        if (recImageFolderCurrentDir < 0) {
            recImageFolderCurrentDir = dirImageFolder.size()-1;
        }
        isRecordImageFolderD = false;
	}
    
    for(int i = 0; i < (int)dirImageFolder.size(); i++){
		if(i == recImageFolderCurrentDir) {
            if (isKeyDiaImage == false && recImageFolderCurrentDirTest == false){
                setRecordImageFolder = dirImageFolder.getName(i);
            }
		}
	}
    //----Directory-ImageFolder---_
    
    //Directory-Image-------_
    if (dirARMImage.size() > 0 && isImageR == true){
		currentARMImage++;
		currentARMImage %= dirARMImage.size();
        isImageR = false;
	}
    if (dirARMImage.size() > 0 && isImageL == true){
		currentARMImage--;
        if (currentARMImage < 0) {
            currentARMImage = dirARMImage.size()-1;
        }
        isImageL = false;
	}
    for(int i = 0; i < (int)dirARMImage.size(); i++){
		if(i == recImageCurrentDir) {
            if (isKeyDiaImage == false && recImageCurrentDirTest == false){
                setRecordImage = dirARMImage.getName(i);
            }
		}
	}
    
    int posLastSlash = setRecordImageFolder.rfind("/");
    if(posLastSlash > 0){
        if (setRecordImageFolder != setRecordImageFolderTest && setRecordImageFolder != "") {
            dirARMImage.listDir(dirARMImageString);
            dirARMImage.sort();
            if( dirARMImage.size() ){
                images.assign(dirARMImage.size(), ofImage());
            }
            for(int i = 0; i < (int)dirARMImage.size(); i++){
                images[i].loadImage(dirARMImage.getPath(i));
            }
            currentARMImage = 0;
            setRecordImageFolderTest = setRecordImageFolder;
        }
    }
    
    if (setRecordImageFolder != setRecordImageFolderTest && setRecordImageFolder != "" && posLastSlash == 0) {
        dirARMImageStringDefault = "record/media/images/";
        dirARMImageString = dirARMImageStringDefault;
        setDirirARMImageString = setRecordImageFolder;
        dirARMImageString += setDirirARMImageString;
        recRecordWhichTagCon = dirARMImageString += "/";
        dirARMImage.listDir(dirARMImageString);
        dirARMImage.sort();
        if( dirARMImage.size() ){
            images.assign(dirARMImage.size(), ofImage());
        }
        for(int i = 0; i < (int)dirARMImage.size(); i++){
            images[i].loadImage(dirARMImage.getPath(i));
        }
        currentARMImage = 0;
        setRecordImageFolderTest = setRecordImageFolder;
    }
     */
    //----Directory-Image---_
    //-------------ofDirectory-----------_
    
    //recRecordARM------------------_
    recRecordARM_0.updateAddress(audioPlayer_0.getLargePositionMS);
    recRecordARM_0.setRGBA(graphRecARM_R, graphRecARM_G, graphRecARM_B, graphRecARM_A);
    recRecordARM_0.updateInfo(recordSession, recordParticipant, recordAge, recordAmputation, recordAudio);
    recRecordWhoClearTagCon = guiMessXML_0.getMessRecordXMLTag;
    recRecordARM_0.updateClearTagContents(recRecordWhoClearTagCon, recRecordWhichTagCon);
    recRecordARM_0.update(recordFile);
    if(isClearAndSaveDoc == true) {
        recRecordARM_0.clearDoc = true;
        guiMessXML_0.recIndex_Y = guiMessXML_0.maxNumInList;
        isClearAndSaveDoc = false;
    }
    //--------recRecordARM----------_
    
    //guiMessXML------------------------_
    if (isKeyFile == false &&
        isKeySession == false && 
        isKeyParticipant == false && 
        isKeyAudio == false && 
        isKeyRecAge == false && 
        isKeyRecAmputation == false) {//isKeyDiaImage == false
        isKeyFill = false;
    }else {
        isKeyFill = true;
    }
    if (guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList) {
        isKeyFile = isKeySession = isKeyParticipant = isKeyAudio = isKeyRecAge = isKeyRecAmputation = false;//isKeyDiaImage
    }
    if (recRecordARM_0.allCamera.size() > 0) {
        
        //Directory-Audio-------_
        nameOfDirPathAudio = "record/media/audio/";
        nameOfDirPathAudio += recRecordARM_0.guiAudio;
        //----Directory-Audio---_
        
        //Directory-ImageFolder-------_
        /*
        string settingsImageDirectory = recRecordARM_0.guiRecARMImages;// setRecordImage;
        int posLastSlashImageDirectory = settingsImageDirectory.rfind("/");
        if(posLastSlashImageDirectory > 0)settingsImageDirectory.erase(settingsImageDirectory.begin()+posLastSlashImageDirectory, settingsImageDirectory.end());
        else settingsImageDirectory = "";
        
        posLastSlashImageDirectory = settingsImageDirectory.rfind("/");
        char thisStringImage = settingsImageDirectory.find_last_of("/\\");
        string nameOfFileImage;
        string nameOfDirPathImage;
        if(posLastSlashImageDirectory > 0){
            
            nameOfFileImage = settingsImageDirectory.substr(thisStringImage+1);
            
            nameOfDirPathImage = settingsImageDirectory.substr(0, thisStringImage);
            nameOfDirPathImage += "/";
        }
        */
        //----Directory-ImageFolder---_
        
        if (guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList) {
            guiMessXML_0.file = recRecordARM_0.currentXmlFile;
            guiMessXML_0.dateTime = recRecordARM_0.guiDateTime;
            guiMessXML_0.session = recRecordARM_0.guiSession;
            guiMessXML_0.participant = recRecordARM_0.guiParticipant;
            guiMessXML_0.age = recRecordARM_0.guiAge;
            guiMessXML_0.amputation = recRecordARM_0.guiAmputation;
            guiMessXML_0.audio = recRecordARM_0.guiAudio;//nameOfFileAudio;
            if (isSetAllDefault == true) {
                setRecordFile = setRecordFileDefault = recRecordARM_0.currentXmlFile;
                setRecordSession = setRecordSessionDefault = recRecordARM_0.guiSession;
                setRecordParticipant = setRecordParticipantDefault = recRecordARM_0.guiParticipant;
                setRecordAge = setRecordAgeDefault = recRecordARM_0.guiAge;
                setRecordAmputation = setRecordAmputationDefault = recRecordARM_0.guiAmputation;
                setRecordAudio = setRecordAudioDefault = nameOfDirPathAudio; //recRecordARM_0.guiAudio;//nameOfFileAudio;
                //setRecordImage = setRecordImageDefault = nameOfFileImage;//recRecordARM_0.guiRecARMImages;
                isSetAllDefault = false;
            }
        }
        
        if (guiMessXML_0.recIndex_Y != guiMessXML_0.maxNumInList) {
            
            if (isSetRecordFileDefault == true) {
                guiMessXML_0.file = setRecordFileDefault;
            }else {
                guiMessXML_0.file = setRecordFile;
            }
            guiMessXML_0.dateTime = recRecordARM_0.guiDateTime;
            guiMessXML_0.session = setRecordSession;
            guiMessXML_0.participant = setRecordParticipant;
            guiMessXML_0.age = setRecordAge;
            guiMessXML_0.amputation = setRecordAmputation;
            if (isSetAudioDefault == true) {
                guiMessXML_0.audio = setRecordAudioDefault;
            }else {
                guiMessXML_0.audio = setRecordAudio;
            }
            isSetAllDefault = true;
        }
        
        guiGetRecMessXML_0.cameraValSize = recRecordARM_0.setGetCameraAddress+1;
        guiGetRecMessXML_0.painScoreValSize = recRecordARM_0.setGetPainScoreAddress+1;
        guiGetRecMessXML_0.timeValSize = recRecordARM_0.setGetTimesAddress+1;
        guiGetRecMessXML_0.positionValSize = recRecordARM_0.setGetPositionsAddress_X+1;
        guiGetRecMessXML_0.xAxisValSize = recRecordARM_0.setGetXAxisXAddress+1;
        guiGetRecMessXML_0.yAxisValSize = recRecordARM_0.setGetYAxisXAddress+1;
        guiGetRecMessXML_0.zAxisValSize = recRecordARM_0.setGetZAxisXAddress+1;
        
        guiGetRecMessXML_0.endRecValSize = recRecordARM_0.setGetEndRecAddress+1;
        
        guiGetRecMessXML_0.cameraVal = recRecordARM_0.guiCameraVal;
        guiGetRecMessXML_0.painScoreVal = recRecordARM_0.guiPainScoreVal;
        guiGetRecMessXML_0.timeVal = recRecordARM_0.guiTimeVal;
        
        guiGetRecMessXML_0.positionVal_X = recRecordARM_0.guiPositionXVal;
        guiGetRecMessXML_0.positionVal_Y = recRecordARM_0.guiPositionYVal;
        guiGetRecMessXML_0.positionVal_Z = recRecordARM_0.guiPositionZVal;
        
        guiGetRecMessXML_0.xAxisXVal = recRecordARM_0.guiXAxisXVal;
        guiGetRecMessXML_0.xAxisYVal = recRecordARM_0.guiXAxisYVal;
        guiGetRecMessXML_0.xAxisZVal = recRecordARM_0.guiXAxisZVal;
        
        guiGetRecMessXML_0.yAxisXVal = recRecordARM_0.guiYAxisXVal;
        guiGetRecMessXML_0.yAxisYVal = recRecordARM_0.guiYAxisYVal;
        guiGetRecMessXML_0.yAxisZVal = recRecordARM_0.guiYAxisZVal;
        
        guiGetRecMessXML_0.zAxisXVal = recRecordARM_0.guiZAxisXVal;
        guiGetRecMessXML_0.zAxisYVal = recRecordARM_0.guiZAxisYVal;
        guiGetRecMessXML_0.zAxisZVal = recRecordARM_0.guiZAxisZVal;
        
        guiGetRecMessXML_0.endRecVal = recRecordARM_0.guiEndRecVal;
        
        ofColor isColorEnd_0;
        ofColor isColorEnd_1;
        isColorEnd_0.r = 0;
        isColorEnd_0.g = 255;
        isColorEnd_0.b = 0;
        isColorEnd_1.r = 0;
        isColorEnd_1.g = 200;
        isColorEnd_1.b = 0;
        guiGetRecMessXML_0.recRecordFloatColor = isColorEnd_1;
        if (recRecordARM_0.isCameraAddressEnd == false) {
            guiGetRecMessXML_0.cameraFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.painScoreIntColor = isColorEnd_1;
        }
        if (recRecordARM_0.isTimesAddressEnd == false) {
            guiGetRecMessXML_0.timeFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.timeFloatColor = isColorEnd_1;
        }
        if (recRecordARM_0.isPositionsAddressEnd == false) {
            guiGetRecMessXML_0.positionFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.positionFloatColor = isColorEnd_1;
        }
        if (recRecordARM_0.isXAxisAddressEnd == false) {
            guiGetRecMessXML_0.xAxisFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.xAxisFloatColor = isColorEnd_1;
        }
        if (recRecordARM_0.isYAxisAddressEnd == false) {
            guiGetRecMessXML_0.yAxisFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.yAxisFloatColor = isColorEnd_1;
        }
        if (recRecordARM_0.isZAxisAddressEnd == false) {
            guiGetRecMessXML_0.zAxisFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.zAxisFloatColor = isColorEnd_1;
        }
        if (recRecordARM_0.isCameraAddressEnd == false) {
            guiGetRecMessXML_0.endRecFloatColor = isColorEnd_0;
        }else {
            guiGetRecMessXML_0.endRecFloatColor = isColorEnd_1;
        }
        guiGetRecMessXML_0.cameraFloatColor;
        guiGetRecMessXML_0.painScoreIntColor;
        guiGetRecMessXML_0.timeFloatColor;
        guiGetRecMessXML_0.positionFloatColor;
        guiGetRecMessXML_0.xAxisFloatColor;
        guiGetRecMessXML_0.yAxisFloatColor;
        guiGetRecMessXML_0.zAxisFloatColor;
        guiGetRecMessXML_0.endRecFloatColor;
    }
    guiMessXML_0.update(20, 150, "record/message/message_presets_ARM.xml");
    guiGetRecMessXML_0.update(20, 290, "record/message/message_presets_ARM.xml");
    //-----------guiMessXML-------------_
    
    //audioPlayer--------------_
    audioPlayer_0.update(20, 440, nameOfDirPathAudio);
    //-------audioPlayer-------_
    
    recRecordARM_0.recordCamera = recCamera;
    recRecordARM_0.recordPainScore = recPainScore;
    recRecordARM_0.recordTime = ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
    getAllPositionsXSize = recRecordARM_0.getAllPositionsXSize;
    recRecordARM_0.recordPosition_X = recPosition_X;
    recRecordARM_0.recordPosition_Y = recPosition_Y;
    recRecordARM_0.recordPosition_Z = recPosition_Z;
    recRecordARM_0.recordXAxis_X = recXAxis_X;
    recRecordARM_0.recordXAxis_Y = recXAxis_Y;
    recRecordARM_0.recordXAxis_Z = recXAxis_Z;
    recRecordARM_0.recordYAxis_X = recYAxis_X;
    recRecordARM_0.recordYAxis_Y = recYAxis_Y;
    recRecordARM_0.recordYAxis_Z = recYAxis_Z;
    recRecordARM_0.recordZAxis_X = recZAxis_X;
    recRecordARM_0.recordZAxis_Y = recZAxis_Y;
    recRecordARM_0.recordZAxis_Z = recZAxis_Z;
    
    //xenoRectOne--------------_
    getPositions_X = recRecordARM_0.allPositions_X[setPosAddres];//or xenoPosX.prevOne
    getPositions_Y = recRecordARM_0.allPositions_Y[setPosAddres];
    getPositions_Z = recRecordARM_0.allPositions_Z[setPosAddres];
    //xenoRectOne--------------_
}

//--------------------------------------------------------------
void dataLoggerARM::draw(){
    //recRecordARM------------------_
    recRecordARM_0.draw(graphRecARM_X, graphRecARM_Y, graphRecARM_W, graphRecARM_H);
    //--------recRecordARM----------_
    
    //guiMessXML------------------------_
    guiMessXML_0.draw();
    guiGetRecMessXML_0.draw();
    //-----------guiMessXML-------------_
    
    //Directory-ImageFolder-------_
    /*
	ofSetColor(ofColor::gray);
    if (dirARMImage.size() > 0){
		ofSetColor(ofColor::white);
		images[currentARMImage].draw(300,220);
	}
     
    int posLastSlash = recRecordARM_0.guiRecARMImages.rfind("/");
    if(posLastSlash > 0 && guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList){
        if (recRecordARM_0.guiRecARMImages != setRecordImageFolderTest && recRecordARM_0.guiRecARMImages != "") {
            dirARMImage.listDir(recRecordARM_0.guiRecARMImages);
            dirARMImage.sort();
            if( dirARMImage.size() ){
                images.assign(dirARMImage.size(), ofImage());
            }
            for(int i = 0; i < (int)dirARMImage.size(); i++){
                images[i].loadImage(dirARMImage.getPath(i));
            }
            currentARMImage = 0;
            setRecordImageFolderTest = setRecordImageFolder;
        }
    }
    */
    //----Directory-ImageFolder---_
    
    //audioPlayer--------------_
    audioPlayer_0.draw();
    //-------audioPlayer-------_
}

//--------------------------------------------------------------
void dataLoggerARM::keyPressed  (int key){

    //audioPlayer--------------_
    if (isKeyFill == false)audioPlayer_0.keyPressed(key);
    //-------audioPlayer-------_
    
    if (isKeyFill == false && guiMessXML_0.recIndex_Y != guiMessXML_0.maxNumInList) {
        if(key == 'C'){
            //recRecordARM_0.isRemoveTag = true;//kill this ?
        }
    }
    
    float setWithRan = ofRandom(0.9, 10.9);
    //recCamera = recPainScore = 1;
    //recPosition_X = recPosition_Y = recPosition_Z = setWithRan;
    //recXAxis_X = recXAxis_Y = recXAxis_Z = setWithRan;
    //recYAxis_X = recYAxis_Y = recYAxis_Z = setWithRan;
    //recZAxis_X = recZAxis_Y = recZAxis_Z = setWithRan;
    recRecordARM_0.recordCamera = recCamera;
    //recRecordARM_0.recordPainScore = recPainScore;
    //recRecordARM_0.recordTime = "-1";
    
    recRecordARM_0.recordPosition_X = recPosition_X;
    recRecordARM_0.recordPosition_Y = recPosition_Y;
    recRecordARM_0.recordPosition_Z = recPosition_Z;
    
    recRecordARM_0.recordXAxis_X = recXAxis_X;
    recRecordARM_0.recordXAxis_Y = recXAxis_Y;
    recRecordARM_0.recordXAxis_Z = recXAxis_Z;
    
    recRecordARM_0.recordYAxis_X = recYAxis_X;
    recRecordARM_0.recordYAxis_Y = recYAxis_Y;
    recRecordARM_0.recordYAxis_Z = recYAxis_Z;
    
    recRecordARM_0.recordZAxis_X = recZAxis_X;
    recRecordARM_0.recordZAxis_Y = recZAxis_Y;
    recRecordARM_0.recordZAxis_Z = recZAxis_Z;
    
    //recRecordARM------------------_
    if (isKeyFill == false && guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList) {
        recRecordARM_0.keyPressed(key);
        if(key == OF_KEY_DOWN || key == 359 || key == OF_KEY_UP || key == 357){
            isSetAllDefault = true;
            recRecordARM_0.isGetAll = true;
        }
        isSetRecordFileDefault = isSetAudioDefault = true;
        recAddressCurrentDir = recordAddressDir.size()-1;
        recAudioCurrentDir = recordAudioDir.size()-1;
        //recImageFolderCurrentDir = dirImageFolder.size()-1;
    }
    //--------recRecordARM----------_
    //ofDirectory------------------------_
    //Directory-Address-------_
    if (isKeyFill == false && isKeyFile == false && guiMessXML_0.recIndex_Y == 0) {        
        if(key == OF_KEY_RIGHT || key == 258){
            if (setRecordFile != "") {     
                if (isJustAtRunFile == true) {
                    recAddressCurrentDir = 0;
                    for(int i = 0; i < (int)recordAddressDir.size(); i++){
                        if(i == recAddressCurrentDir) {
                            setRecordFileDefault = setRecordFile = recordFile = recordAddressDir.getName(i);
                        }
                    }
                    isJustAtRunFile = false;
                }
            }
            isKeyFileRL = true;
            recAddressCurrentDirTest = false;
            isSetRecordFileDefault = false;
            isRecordAddressR = true;
        }
        if(key == OF_KEY_LEFT || key == 256){
            if (setRecordFile != "") {     
                if (isJustAtRunFile == true) {
                    recAddressCurrentDir = 0;
                    for(int i = 0; i < (int)recordAddressDir.size(); i++){
                        if(i == recAddressCurrentDir) {
                            setRecordFileDefault = setRecordFile = recordFile = recordAddressDir.getName(i);
                        }
                    }
                    isJustAtRunFile = false;
                }
            }
            isKeyFileRL = true;
            recAddressCurrentDirTest = false;
            isSetRecordFileDefault = false;
            isRecordAddressL = true;
        }
        if(key == OF_KEY_DOWN || key == 359 || key == OF_KEY_UP || key == 357){
            isKeyFileRL = false;
        }
    }
    //---Directory-Address----_
    
    //Directory-Audio-------_
	if (isKeyFill == false && isKeyAudio == false && guiMessXML_0.recIndex_Y == 6) {
        if(key == OF_KEY_RIGHT || key == 258){
            recAudioCurrentDirTest = false;
            isSetAudioDefault = false;
            isRecordAudioR = true;
        }
        if(key == OF_KEY_LEFT || key == 256){
            recAudioCurrentDirTest = false;            
            isSetAudioDefault = false;
            isRecordAudioL = true;
        }
    }
    //----Directory-Audio---_
    
    //Directory-ImageFolder-------_
    /*
    if (isKeyFill == false && isKeyDiaImage == false && guiMessXML_0.recIndex_Y == 9) {
        if(key == OF_KEY_RIGHT || key == 258){
            recImageFolderCurrentDirTest = false;
            isSetARMImagesFolderDefault = false;
            isSetARMImagesDefault = false;
            isRecordImageFolderUp = true;
        }
        if(key == OF_KEY_LEFT || key == 256){
            recImageFolderCurrentDirTest = false;
            isSetARMImagesFolderDefault = false;
            isSetARMImagesDefault = false;
            isRecordImageFolderD = true;
        }
    }
    //----Directory-ImageFolder---_
    
    //Directory-Image-------_
    if (isKeyFill == false && isKeyDiaImage == false && guiMessXML_0.recIndex_Y == 9) {
        if(key == OF_KEY_UP || key == 357){
            recImageCurrentDirTest = false;
            isSetARMImagesDefault = false;
            isImageR = true;
        }
        if(key == OF_KEY_DOWN || key == 359){
            recImageCurrentDirTest = false;
            isSetARMImagesDefault = false;
            isImageL = true;
        }
    }
    */
    //----Directory-Image---_
    //-------------ofDirectory-----------_
    
    //letterForm----------------------_
    if ( guiMessXML_0.recIndex_Y != 0
        && guiMessXML_0.recIndex_Y != 6
        && guiMessXML_0.recIndex_Y != guiMessXML_0.maxNumInList
        && isKeyFill == false) {
        if (key == OF_KEY_RIGHT || key == 258) {
            guiMessXML_0.isRight = true;
        } 
        if (key == OF_KEY_LEFT || key == 256) {
            guiMessXML_0.isLeft = true;
        }
    }

    if(key == '<') {
		guiMessXML_0.isDownList = true;
        if (guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList) {
            recordAddressDir.listDir("record/address/");
            recordAddressDir.sort();
        }
        isSetAllDefault = true;
        isKeyFileRL = isKeyAudioRL = false;
        recRecordARM_0.isGetAll = true;
	} else if (key == '>' || key == 9) {
        guiMessXML_0.isUpList = true;
        if (guiMessXML_0.recIndex_Y == guiMessXML_0.maxNumInList) {
            recordAddressDir.listDir("record/address/");
            recordAddressDir.sort();
        }
        isKeyFileRL = isKeyAudioRL = false;
        isSetAllDefault = true;
        recRecordARM_0.isGetAll = true;
	} else if(key == OF_KEY_RETURN || key == 13) {
        addKeyIndexZero = 0;
        toAddZero = "";
        isDelAtIndexZero = false;
        guiMessXML_0.isSetCursorAtSizeZero_X = false;
        
        if (guiMessXML_0.recIndex_Y == 0) {
            if (setRecordFile != "" && isKeyFileRL == true) {  
                if (isJustAtRunFile == true) {
                    recAddressCurrentDir = 0;
                    for(int i = 0; i < (int)recordAddressDir.size(); i++){
                        if(i == recAddressCurrentDir) {
                            setRecordFileDefault = setRecordFile = recordFile = recordAddressDir.getName(i);
                        }
                    }
                    isJustAtRunFile = false;
                }else {
                    int numFileNameMach = 0;
                    bool isTested = false;
                    for(int i = 0; i < (int)recordAddressDir.size(); i++){
                        if(setRecordFile == recordAddressDir.getName(i)) {
                            numFileNameMach++;
                        }
                        if (i == (int)recordAddressDir.size()-1) {
                            isTested = true;
                        }
                    }
                    if (numFileNameMach >= 1) {
                        isClearAndSaveDoc = false;
                        guiMessXML_0.recIndex_Y = guiMessXML_0.maxNumInList;
                    }
                    if (numFileNameMach <= 0 && isTested == true){
                        isClearAndSaveDoc = true;
                    }
                    recRecordWhichTagCon = recordFile = setRecordFile;
                }
                //ofDirectory--------_
                recordAddressDir.listDir("record/address/");
                recordAddressDir.sort();
                //----ofDirectory----_
                isKeyFileRL = false;
                isKeyFile = false;
            }
            recRecordARM_0.isSaveAll = true;
            recRecordARM_0.isGetAll = true;
            guiMessXML_0.isFileEnter = false;
        }
        //---ofDirectory-----_
        
        if (guiMessXML_0.recIndex_Y == 2) {
            if (setRecordSession.at(0) == ' ') {
                setRecordSession.erase(setRecordSession.begin());
            }
            recRecordWhichTagCon = recordSession = setRecordSession;
            recRecordARM_0.isRemoveTag = true;
            isKeySession = false;
            guiMessXML_0.isSessionEnter = false;
        }
        if (guiMessXML_0.recIndex_Y == 3) {
            if (setRecordParticipant.at(0) == ' ') {
                setRecordParticipant.erase(setRecordParticipant.begin());
            }
            recRecordWhichTagCon = recordParticipant = setRecordParticipant;
            recRecordARM_0.isRemoveTag = true;
            isKeyParticipant = false;
            guiMessXML_0.isParticipantEnter = false;
        }
        if (guiMessXML_0.recIndex_Y == 4) {
            if (setRecordAge.at(0) == ' ') {
                setRecordAge.erase(setRecordAge.begin());
            }
            recRecordWhichTagCon = recordAge = setRecordAge;
            recRecordARM_0.isRemoveTag = true;
            isKeyRecAge = false;
            guiMessXML_0.isAgeEnter = false;
        }
        if (guiMessXML_0.recIndex_Y == 5) {
            if (setRecordAmputation.at(0) == ' ') {
                setRecordAmputation.erase(setRecordAmputation.begin());
            }
            recRecordWhichTagCon = recordAmputation = setRecordAmputation;
            recRecordARM_0.isRemoveTag = true;
            isKeyRecAmputation = false;
            guiMessXML_0.isAmputationEnter = false;
        }
        if (guiMessXML_0.recIndex_Y == 6) {
            if (setRecordAudio != "" && isKeyAudioRL == true){
                recRecordWhichTagCon = recordAudio = setRecordAudio;
                //ofDirectory--------_
                recordAudioDir.listDir("record/media/audio/");
                recordAudioDir.sort();
                //---ofDirectory-----_
                isKeyAudioRL = false;
                recRecordARM_0.isRemoveTag = true;
                isKeyAudio = false;
                guiMessXML_0.isAudioEnter = false;
            }
        }
        /*
        if (guiMessXML_0.recIndex_Y == 7) {
            if (setRecordImageFolder != "") {
                recordImage = setRecordImageFolder;//setRecordImage;
                //ofDirectory--------_
                dirARMImageStringDefault = "record/media/images/";
                dirARMImageString = dirARMImageStringDefault;
                setDirirARMImageString = setRecordImageFolder;
                dirARMImageString += setDirirARMImageString;
                recRecordWhichTagCon = dirARMImageString += "/";
                dirARMImage.listDir(dirARMImageString);
                dirARMImage.sort();
                
                //allocate the vector to have as many ofImages as files
                if( dirARMImage.size() ){
                    images.assign(dirARMImage.size(), ofImage());
                }
                // you can now iterate through the files and load them into the ofImage vector
                for(int i = 0; i < (int)dirARMImage.size(); i++){
                    images[i].loadImage(dirARMImage.getPath(i));
                }
                currentARMImage = 0;
                setRecordImageFolderTest = dirARMImageString;
                //---ofDirectory-----_
            }
            isKeyDiaImage = false;
        }
         */
	}else if(key == OF_KEY_DEL || key == 127) {
        addKeyIndexZero = 0;
        toAddZero = "";
        
        if (guiMessXML_0.recIndex_Y == 0) {
            if (isJustAtRunFile == true) {
                isJustAtRunFile = false;
            }
            setRecordFile = "";
            //ofDirectory--------_
            recAddressCurrentDirTest = true;
            isSetRecordFileDefault = false;
            //----ofDirectory----_
            isKeyFileRL = true;
            isKeyFile = true;
            guiMessXML_0.isFileEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 2) {
            if (setRecordSession.size() > 0) {
                if (guiMessXML_0.stringIndexRightLeft == 0) {
                    isDelAtIndexZero = true;
                }
                if (guiMessXML_0.stringIndexRightLeft >= setRecordSession.size()) {
                    setRecordSession.erase(setRecordSession.end()-1,setRecordSession.end());
                    guiMessXML_0.stringIndexRightLeft--;
                }else {
                    setRecordSession.erase(setRecordSession.begin()+guiMessXML_0.stringIndexRightLeft);
                    guiMessXML_0.stringIndexRightLeft--;
                }
            }
            if (setRecordSession.size() <= 0) {
                isDelAtIndexZero = false;
                setRecordSession = "";
            }
            guiMessXML_0.stringSize = setRecordSession.size();
            stringSizeWas = setRecordSession.size();
            isKeySession = true;
            guiMessXML_0.isSessionEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 3) {
            if (setRecordParticipant.size() > 0) {
                if (guiMessXML_0.stringIndexRightLeft == 0) {
                    isDelAtIndexZero = true;
                }
                if (guiMessXML_0.stringIndexRightLeft >= setRecordParticipant.size()) {
                    setRecordParticipant.erase(setRecordParticipant.end()-1,setRecordParticipant.end());
                    guiMessXML_0.stringIndexRightLeft--;
                }else {
                    setRecordParticipant.erase(setRecordParticipant.begin()+guiMessXML_0.stringIndexRightLeft);
                    guiMessXML_0.stringIndexRightLeft--;
                }
            }
            if (setRecordParticipant.size() <= 0) {
                isDelAtIndexZero = false;
                setRecordParticipant = "";
            }
            guiMessXML_0.stringSize = setRecordParticipant.size();
            stringSizeWas = setRecordParticipant.size();

            isKeyParticipant = true;
            guiMessXML_0.isParticipantEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 4) {
            if (setRecordAge.size() > 0) {
                if (guiMessXML_0.stringIndexRightLeft == 0) {
                    isDelAtIndexZero = true;
                }
                if (guiMessXML_0.stringIndexRightLeft >= setRecordAge.size()) {
                    setRecordAge.erase(setRecordAge.end()-1,setRecordAge.end());
                    guiMessXML_0.stringIndexRightLeft--;
                }else {
                    setRecordAge.erase(setRecordAge.begin()+guiMessXML_0.stringIndexRightLeft);
                    guiMessXML_0.stringIndexRightLeft--;
                }
            }
            if (setRecordAge.size() <= 0) {
                isDelAtIndexZero = false;
                setRecordAge = "";
            }
            guiMessXML_0.stringSize = setRecordAge.size();
            stringSizeWas = setRecordAge.size();
            isKeyRecAge = true;
            guiMessXML_0.isAgeEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 5) {
            if (setRecordAmputation.size() > 0) {
                if (guiMessXML_0.stringIndexRightLeft == 0) {
                    isDelAtIndexZero = true;
                }
                if (guiMessXML_0.stringIndexRightLeft >= setRecordAmputation.size()) {
                    setRecordAmputation.erase(setRecordAmputation.end()-1,setRecordAmputation.end());
                    guiMessXML_0.stringIndexRightLeft--;
                }else {
                    setRecordAmputation.erase(setRecordAmputation.begin()+guiMessXML_0.stringIndexRightLeft);
                    guiMessXML_0.stringIndexRightLeft--;
                }
            }
            if (setRecordAmputation.size() <= 0) {
                isDelAtIndexZero = false;
                setRecordAmputation = "";
            }
            guiMessXML_0.stringSize = setRecordAmputation.size();
            stringSizeWas = setRecordAmputation.size();
            isKeyRecAmputation = true;
            guiMessXML_0.isAmputationEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 6) {
            //ofDirectory--------_
            recAudioCurrentDirTest = true;
            isSetAudioDefault = false;
            //----ofDirectory----_
            guiMessXML_0.isAudioEnter = true;
        }
        /*
        if (guiMessXML_0.recIndex_Y == 7) {
            //setRecordImageFolder = setRecordImage = "";
            //ofDirectory--------_
            recImageFolderCurrentDirTest = true;
            isSetARMImagesFolderDefault = false;
            recImageCurrentDirTest = true;
            isSetARMImagesDefault = false;
            //----ofDirectory----_
            //isKeyDiaImage = true;
        }
         */
	} else if(key != '@' 
              && key != OF_KEY_DEL && key != 127 
              && key != OF_KEY_RETURN && key != 13 
              && key != OF_KEY_RIGHT && key != 258 
              && key != OF_KEY_LEFT && key != 256
              && key != OF_KEY_UP && key != 357
              && key != OF_KEY_DOWN && key != 359) {
		letterForm = key;
        if (guiMessXML_0.recIndex_Y == 0 && 
            key != ':'&& 
            key != ';'&& 
            key != '?'&&
            key != '>'&& 
            key != '<'&& 
            key != '{'&& 
            key != '}'&& 
            key != '['&& 
            key != ']'&& 
            key != '"'&& 
            key != ')'&& 
            key != '('&& 
            key != '*'&& 
            key != '&'&& 
            key != '^'&& 
            key != '%'&& 
            key != '$'&& 
            key != '#'&& 
            key != '@'&& 
            key != '!'&& 
            key != '.'&& 
            key != ','&& 
            key != '~'&& 
            key != '`'&& 
            key != '+'&& 
            key != '='&& 
            key != '|'&& 
            key != 92 && 
            key != 27 && 
            key != ' '&& 
            key != 9) {
            if (isKeyFile == true)setRecordFile += letterForm;
            guiMessXML_0.isFileEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 2) {
            if (setRecordSession.size() > 0) {
                addKeyIndexZero++;
                cout << addKeyIndexZero << endl;
                if (isDelAtIndexZero == true) {
                    guiMessXML_0.stringIndexRightLeft++;
                    toAddZero += letterForm;
                    if (toAddZero.size() > 1) {
                        toAddZero.erase(toAddZero.begin()+addKeyIndexZero, toAddZero.end());
                    }
                    setRecordSession.insert(0, toAddZero);
                    setRecordSession.erase(setRecordSession.begin(), setRecordSession.end()-stringSizeWas);
                    setRecordSession.insert(0, toAddZero);
                    
                }
                if (isDelAtIndexZero == false) {
                    guiMessXML_0.stringIndexRightLeft++;
                    setRecordSession.insert(guiMessXML_0.stringIndexRightLeft, letterForm);
                }
            }
            
            if (setRecordSession.size() <= 0) {
                guiMessXML_0.stringIndexRightLeft = 0;
                setRecordSession += letterForm;
            }
            guiMessXML_0.stringSize = setRecordSession.size();
            guiMessXML_0.isSessionEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 3) {
            if (setRecordParticipant.size() > 0) {
                addKeyIndexZero++;
                cout << addKeyIndexZero << endl;
                if (isDelAtIndexZero == true) {
                    guiMessXML_0.stringIndexRightLeft++;
                    toAddZero += letterForm;
                    if (toAddZero.size() > 1) {
                        toAddZero.erase(toAddZero.begin()+addKeyIndexZero, toAddZero.end());
                    }
                    setRecordParticipant.insert(0, toAddZero);
                    setRecordParticipant.erase(setRecordParticipant.begin(), setRecordParticipant.end()-stringSizeWas);
                    setRecordParticipant.insert(0, toAddZero);
                    
                }
                if (isDelAtIndexZero == false) {
                    guiMessXML_0.stringIndexRightLeft++;
                    setRecordParticipant.insert(guiMessXML_0.stringIndexRightLeft, letterForm);
                }
            }
            
            if (setRecordParticipant.size() <= 0) {
                guiMessXML_0.stringIndexRightLeft = 0;
                setRecordParticipant += letterForm;
            }
            guiMessXML_0.stringSize = setRecordParticipant.size();
            guiMessXML_0.isParticipantEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 4) {
            if (setRecordAge.size() > 0) {
                addKeyIndexZero++;
                cout << addKeyIndexZero << endl;
                if (isDelAtIndexZero == true) {
                    guiMessXML_0.stringIndexRightLeft++;
                    toAddZero += letterForm;
                    if (toAddZero.size() > 1) {
                        toAddZero.erase(toAddZero.begin()+addKeyIndexZero, toAddZero.end());
                    }
                    setRecordAge.insert(0, toAddZero);
                    setRecordAge.erase(setRecordAge.begin(), setRecordAge.end()-stringSizeWas);
                    setRecordAge.insert(0, toAddZero);
                    
                }
                if (isDelAtIndexZero == false) {
                    guiMessXML_0.stringIndexRightLeft++;
                    setRecordAge.insert(guiMessXML_0.stringIndexRightLeft, letterForm);
                }
            }
            
            if (setRecordAge.size() <= 0) {
                guiMessXML_0.stringIndexRightLeft = 0;
                setRecordAge += letterForm;
            }
            guiMessXML_0.stringSize = setRecordAge.size();
            guiMessXML_0.isAgeEnter = true;
        }
        if (guiMessXML_0.recIndex_Y == 5) {
            if (setRecordAmputation.size() > 0) {
                addKeyIndexZero++;
                cout << addKeyIndexZero << endl;
                if (isDelAtIndexZero == true) {
                    guiMessXML_0.stringIndexRightLeft++;
                    toAddZero += letterForm;
                    if (toAddZero.size() > 1) {
                        toAddZero.erase(toAddZero.begin()+addKeyIndexZero, toAddZero.end());
                    }
                    setRecordAmputation.insert(0, toAddZero);
                    setRecordAmputation.erase(setRecordAmputation.begin(), setRecordAmputation.end()-stringSizeWas);
                    setRecordAmputation.insert(0, toAddZero);
                    
                }
                if (isDelAtIndexZero == false) {
                    guiMessXML_0.stringIndexRightLeft++;
                    setRecordAmputation.insert(guiMessXML_0.stringIndexRightLeft, letterForm);
                }
            }
            
            if (setRecordAmputation.size() <= 0) {
                guiMessXML_0.stringIndexRightLeft = 0;
                setRecordAmputation += letterForm;
            }
            guiMessXML_0.stringSize = setRecordAmputation.size();
            guiMessXML_0.isAmputationEnter = true;
        }
	}   
    //---------letterForm-------------_
}
