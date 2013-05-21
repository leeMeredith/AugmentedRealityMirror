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
    
    //ofCamera----------------------------------------_
    void reset(); 
    
    ofNode testNodes[kNumTestNodes];
    //ofLight light[kNumLights];
    ofCamera cam[kNumCameras];
    int lookatIndex[kNumCameras]; // which test node to lookat (one for each camera)
    int parentIndex[kNumCameras]; // which test node to link (parent) to (one for each camera)
    bool doMouseOrbit[kNumCameras]; // whether camera is in mouseOrbit mode or not
    float orbitRadius;
    
    int camToView; // which camera index are we looking through
    int camToConfigure; // which camera index we are configuring
    bool isWhoKey;
    bool isGuiCamera;
    //--------------------ofCamera--------------------_
    
    //dataLoggerARM-----------------------------------_
    bool isGuiDataLoggerARM, isDataLoggerARM;
    dataLoggerARM dataLoggerARM_0;
    //-----------------dataLoggerARM------------------_
    
    //velSmoothStateXY--------------------------------_
    velSmoothStateXY velSmoothStateMouseMoved;
    //-----------------velSmoothStateXY---------------_
    
    //drHitAreaImage----------------------------------_
    drHitAreaImage drHitAreaImage_0;
    //-----------------drHitAreaImage-----------------_
    
};
