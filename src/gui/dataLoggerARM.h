//
//  Created by Lee Meredith.
//

#pragma once

#include "ofxXmlSettings.h"
#include "recRecordARM.h"
#include "guiMessXML.h"
#include "guiGetRecMessXML.h"
#include "audioPlayer.h"

class dataLoggerARM{

public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    float recordAddress;
    int setRecordAddress, setEventsAddress;
    bool isGetEvents;
    bool isSetAllDefault;
    bool isSetRecordFileDefault, isSetAudioDefault, isSetARMImagesDefault, isSetARMImagesFolderDefault;
    string date;
    string setRecordFileDefault, setRecordFile, recordFile;
    string setRecordAudioDefault, setRecordAudio, recordAudio;
    string setRecordImageDefault, setRecordImage, recordImage;
    string setRecordImageFolderDefault, setRecordImageFolder, recordImageFolder, setRecordImageFolderTest;
    string recordSession, recordParticipant, recordAge, recordAmputation;
    string setRecordSession, setRecordParticipant, setRecordAge, setRecordAmputation;
    string setRecordSessionDefault, setRecordParticipantDefault, setRecordAgeDefault, setRecordAmputationDefault;
    
    bool isKeyFile, isKeySession, isKeyParticipant, isKeyAudio, isKeyRecAge, isKeyRecAmputation;// isKeyDiaImage;
    bool isKeyFill;
    bool isRecord, isSaveAll, isSaveEvents, isStartEvents, isEventsTest, isNothing, isARM;
    bool clearDoc, clearTagCon, clearTagAttrib;
    
    bool isGetAll;
    
    float graphRecARM_X, graphRecARM_Y, graphRecARM_W, graphRecARM_H;
    int graphRecARM_R, graphRecARM_G, graphRecARM_B, graphRecARM_A;
    int setGetRecordAddress, setGetRecordAddressAll, setGetEventsAddress, setGetWordsAddress, setGetLettersAddress, setGetARMAddress, setGetNothingsAddress;
    
    string recRecordWhoClearTagCon, recRecordWhichTagCon;
    
    recRecordARM recRecordARM_0;
    int recCamera, recPainScore, getAllPositionsXSize;
    float recPosition_X, recPosition_Y, recPosition_Z, recXAxis_X, recXAxis_Y, recXAxis_Z, recYAxis_X, recYAxis_Y, recYAxis_Z, recZAxis_X, recZAxis_Y, recZAxis_Z;
    
    //guiMessXML------------------------_
    bool isDelAtIndexZero;
    int addKeyIndexZero;
    int stringSizeWas;
    string toAddZero;
    guiMessXML guiMessXML_0;
    guiGetRecMessXML guiGetRecMessXML_0;
    //-----------guiMessXML-------------_
    
    //letterForm----------------------_
    string letterForm;
    //-----------letterForm-----------_
    
    //ofDirectory------------------------_
    ofDirectory recordAddressDir;
    bool isJustAtRunFile;
    int recAddressCurrentDir, recAddressCurrentDirTest;
    bool isRecordAddressR, isRecordAddressL;
    bool isKeyAudioRL, isKeyFileRL;
    bool isClearAndSaveDoc;
    
    ofDirectory recordAudioDir;
    int recAudioCurrentDir, recAudioCurrentDirTest;
    bool isRecordAudioR, isRecordAudioL;
    string nameOfDirPathAudio;
    /*
    ofDirectory dirImageFolder;
    bool isJustAtRunImageFolder;
    int recImageFolderCurrentDir, recImageFolderCurrentDirTest;
    bool isRecordImageFolderD, isRecordImageFolderUp;
    string dirARMImageFolderStringDefault, dirARMImageFolderString, setDirirARMImageFolderString;
    
    vector<string> imagesFolder;
    int currentARMImageFolder;
    bool isImageFolderR, isImageFolderL;
    
    ofDirectory dirARMImage;
    int recImageCurrentDir, recImageCurrentDirTest;
    string dirARMImageStringDefault, dirARMImageString, setDirirARMImageString;
    
    vector<ofImage> images;
    int currentARMImage;
    bool isImageR, isImageL;
     */
    //-------------ofDirectory-----------_
    
    //audioPlayer--------------_
    audioPlayer audioPlayer_0;
    //-------audioPlayer-------_
    
    //xenoRectOne--------------_
    int setPosAddres;
    float getPositions_X, getPositions_Y, getPositions_Z;    
    //xenoRectOne--------------_
};

