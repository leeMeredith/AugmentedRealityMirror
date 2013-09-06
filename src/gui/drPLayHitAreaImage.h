#pragma once

#include "ofMain.h"
#include "playbackHitAreaImage.h"

#define MAX_NUM_IMAGE 4

class drPLayHitAreaImage{
	public:
		void setup();
		void update();
        void reset();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
        
        float mouseX, mouseY;
        float width, height;
        float newBoundingBoxHeight;
        float setX_0, setY_0, setX_1, setY_1;
        int menuMode, testMenuMode;
        bool isHitOk, isReset;
    
    
        //-------------ofDirectory-----------_
        string setRecordImageDefault, setRecordImage, recordImage;
        string setRecordImageFolderDefault, setRecordImageFolder, recordImageFolder, setRecordImageFolderTest;
        ofDirectory dirDialogFolder;
        bool isJustAtRunDialogFolder;
        int recImageFolderCurrentDir, recImageFolderCurrentDirTest;
        bool isRecordImageFolderD, isRecordImageFolderUp;
        string dirDialogImageFolderStringDefault, dirDialogImageFolderString, setDirirDialogImageFolderString;
        
        vector<string> imagesFolder;
        int currentDialogImageFolder;
        bool isImageFolderR, isImageFolderL;
        
        ofDirectory dirDialogImage;
        int recImageCurrentDir, recImageCurrentDirTest;
        string dirDialogImageStringDefault, dirDialogImageString, setDirirDialogImageString;
        
        vector<ofImage> images;
        int currentDialogImage;
        bool isImageR, isImageL;
        
        int hitAreaImageIndex;
        bool isAddHitDialog;
        string hitDialogIndex;
        ofColor testSelPPMode_0, testInPPMode_0;
        ofColor testSelPPMode_1, testInPPMode_1;
        ofColor testSelRecMode_0, testInRecMode_0;
        ofColor testSelRecMode_1, testInRecMode_1;
        ofImage myImage[MAX_NUM_IMAGE];
        playbackHitAreaImage playbackHitAreaImage_0;
        //-------------ofDirectory-----------_
};
