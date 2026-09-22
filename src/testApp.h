//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "dataLoggerARM.h"
#include "velSmoothStateXY.h"
#include "drHitAreaImage.h"
#include "playbackCam.h"
#include "drPLayHitAreaImage.h"
#include "timer.h"

#define kNumTestNodes 4
#define kNumCameras 2
//#define kNumLights 3

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
    void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxKinect kinect;
	
    bool  	bFullscreen, isDrawOpenCV, isSession;
    
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
    bool isScaleSessions;
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
    
    bool isGuiKinect;
    bool isGuiAll;
    bool dBug;
    int flipH;
    
    bool isMouseMovTop, isMouseMovBott;
    int mouseMovTop, mouseMovBott;
    int mouseMovTop_y, mouseMovBott_Y;
    
    
    //ofCamera----------------------------------------_
    void reset(); 
    
    ofNode testNodes[kNumTestNodes];
    //ofLight light[kNumLights];
    ofCamera cam[kNumCameras];
    int lookatIndex[kNumCameras]; // which test node to lookat (one for each camera)
    int parentIndex[kNumCameras]; // which test node to link (parent) to (one for each camera)
    bool doMouseOrbit[kNumCameras]; // whether camera is in mouseOrbit mode or not
    float orbitRadius, setOrbitRadius;
    
    int camToView; // which camera index are we looking through
    int camToConfigure; // which camera index we are configuring
    bool isWhoKey;
    bool isGuiCamera;
    float setLon, setLat;
    //--------------------ofCamera--------------------_
    
    //dataLoggerARM-----------------------------------_
    bool isGuiDataLoggerARM, isDataLoggerARM;
    bool isCamMove, isCamMoveTest;
    int setPosAddres, camMove, setPosAddresTest;
    float newRecPos_X, newRecPosTest_X;
    float newRecPos_Y, newRecPosTest_Y;
    float newRecPos_Z, newRecPosTest_Z;
    dataLoggerARM dataLoggerARM_0;
    //-----------------dataLoggerARM------------------_
    
    //velSmoothStateXY--------------------------------_
    velSmoothStateXY velSmoothStateMouseMoved;
    //-----------------velSmoothStateXY---------------_
    
    //drHitAreaImage----------------------------------_
    bool isDrHitAreaImage;
    drHitAreaImage drHitAreaImage_0;
    //-----------------drHitAreaImage-----------------_
    
    //drPLayHitAreaImage------------------------------_
    bool isDrPLayHitAreaImage, isPlay, isPlayReset, isPlayResetTest;
    bool isRR, isRRTest, isFF, isFFTest;
    bool isRec, isRecTest, isRecReset, isRecResetTest;
    int playMode, rrMode, ffMode, recMode;
    drPLayHitAreaImage drPLayHitAreaImage_0;
    //-----------------drPLayHitAreaImage-------------_
    
    //playbackCam-------------------------------------_
    float newPos_X, newPos_Y, newPos_Z;
    float newPosTest, newPosTest_X, newPosTest_Y, newPosTest_Z;
    int cameraMode;
    float rampAmount;
    playbackCam playbackCam_0;
    //-----------------playbackCam--------------------_
    
    //timer-------------------------------------------_
    bool isTimerStart, isTimerStartTest;
    int howLongPlaybackCam, howManyPlaybackCam;
    timer playbackCamTimer_0;
    //---------------------timer----------------------_
    
};
