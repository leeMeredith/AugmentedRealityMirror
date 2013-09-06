//
//  Created by Lee Meredith.
//

#include "testApp.h"

//ofCamera----------------------------------------_
#define kMoveInc 10
#define kRotInc 5

// reset all transformations and options to defaults
void testApp::reset() {
	camToView = 0;
	camToConfigure = 0;
	
	orbitRadius = 500;
	
	for(int i=0; i<kNumCameras; i++) {
		cam[i].resetTransform();
		cam[i].setFov(60);
		cam[i].clearParent();
		lookatIndex[i] = -1; // don't lookat at any node
		parentIndex[i] = -1; // don't parent to any node
		doMouseOrbit[i] = false;
	}
	
	cam[0].setPosition(40, 40, 190);
	doMouseOrbit[0] = true;
	
	cam[1].setPosition(80, 40, 30);
	lookatIndex[1] = kNumTestNodes-4; // look at smallest node
}

//--------------------ofCamera--------------------_

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("ARM");//"Aaugmented Reality Mirror"
    ofSetWindowPosition(10, 10);
    bFullscreen	= 0;
    
    isMouseMovTop = isMouseMovBott = false;
    //mouseMovTop = mouseMovBott = 0;
    mouseMovTop_y = 95;
    mouseMovBott_Y = ofGetHeight()-130;
    
	//ofCamera----------------------------------------_
    glEnable(GL_DEPTH_TEST);
	ofSetVerticalSync(true);
	//ofEnableLighting();
    //--------------------ofCamera--------------------_

	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
    flipH = 0;

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    isScaleSessions = false;
    
    //ofCamera----------------------------------------_
	//for(int i = 0; i < kNumLights; i++) {
	//	light[i].enable();
	//}
	
	reset();
	
	// link all testNodes (parent to each other)
	for(int i=0; i<kNumTestNodes; i++) {
		if(i>0) testNodes[i].setParent(testNodes[i-1]);
	}
    //--------------------ofCamera--------------------_
    
    //dataLoggerARM-----------------------------------_
    isDataLoggerARM = isGuiDataLoggerARM = false;
    isGuiAll = true;
    isCamMove = true;
    //recPositionNew = recPositionNewTest = 0;
    newRecPos_X = newRecPosTest_X = 0;
    newRecPos_Y = newRecPosTest_Y = 0;
    newRecPos_Z = newRecPosTest_Z = 0;
    dataLoggerARM_0.setup();
    //-----------------dataLoggerARM------------------_
    
    //velSmoothStateXY--------------------------------_
    velSmoothStateMouseMoved.setup(5, 5);
    //-----------------velSmoothStateXY---------------_
    
    //drHitAreaImage----------------------------------_
    isDrHitAreaImage = true;
    drHitAreaImage_0.setup();
    //-----------------drHitAreaImage-----------------_
    
    //drPLayHitAreaImage------------------------------_
    isDrPLayHitAreaImage = false;
    isPlay = true;
    isRR = isRRTest = isFF = isFFTest = false;
    isRec = false;
    isRecTest = true;
    recMode = playMode = 0;
    isRecReset = isPlayReset = false;
    drPLayHitAreaImage_0.setup();
    //-----------------drPLayHitAreaImage-------------_
    
    //playbackCam-------------------------------------_
    newPosTest_X = newPos_X = 0;
    newPosTest_Y = newPos_Y = 0;
    newPosTest_Z = newPos_Z = 0;
    rampAmount = 0.05;
    setPosAddres = setPosAddresTest = -1;
    playbackCam_0.setup();
    //-----------------playbackCam--------------------_
    
    //timer------------_
    isTimerStart = isTimerStartTest = false;
    howLongPlaybackCam = 25;
    howManyPlaybackCam = 1;
	playbackCamTimer_0.setup(65, 2);//? need to be in 5's (35-25-15-?, 2) or misses
	playbackCamTimer_0.howManyTimeTop = 3;
	playbackCamTimer_0.howManyTimeInSide = 6;
	//-----timer-------_

}

//--------------------------------------------------------------
void testApp::update() {
	
	ofBackground(100, 100, 100);
	
	kinect.update();
    if(!bFullscreen){
        ofSetWindowTitle("ARM");//"Aaugmented Reality Mirror"
    }
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		//contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
	}
    
    //ofCamera----------------------------------------_
    float freqMult = 1;
	float amp = 10;
	float scale = 1;
	
    testNodes[0].setPosition(ofVec3f(0, 0, 0));
    testNodes[1].setPosition(ofVec3f(0, 0, 0));
    testNodes[2].setPosition(ofVec3f(0, 0, 0));
    testNodes[3].setPosition(ofVec3f(0, 0, 0));
	// all testNodes move in simple circles
	// but because they are parented to each other, complex paths emerge
	//for(int i=0; i<kNumTestNodes; i++) {
		//testNodes[i].setPosition(ofVec3f(sin(ofGetElapsedTimef() * freqMult) * amp, cos(ofGetElapsedTimef() * freqMult) * amp, sin(ofGetElapsedTimef() * freqMult * 0.7) * amp));
		
		//testNodes[i].setOrientation(ofVec3f(sin(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, cos(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, sin(ofGetElapsedTimef() * freqMult * 0.2 * 0.7) * amp * 5));
        //testNodes[i].setScale(scale);
        //
        //freqMult *= 3;
        //amp *= 0.8;
        //scale *= 0.8;
	//}
    //---------------------ofCamera-------------------_
    
    //dataLoggerARM-----------------------------------_
    if (isGuiAll == true) {
        isScaleSessions = false;
    }else {
        isScaleSessions = true;
    }
    
//    if (isGuiDataLoggerARM == true && bDrawPointCloud == true && isGuiAll == true) {
//        isDataLoggerARM = true;
//    }
    
    if (isDataLoggerARM == true) {
        ofNode *n = &cam[camToView];
        dataLoggerARM_0.recCamera = camToView;
        //cout << " n->getX() " << n->getX() << endl;
        dataLoggerARM_0.recPosition_X = n->getX();
        dataLoggerARM_0.recPosition_Y = n->getY();
        dataLoggerARM_0.recPosition_Z = n->getZ();
        dataLoggerARM_0.recXAxis_X = n->getXAxis().x;
        dataLoggerARM_0.recXAxis_Y = n->getXAxis().y;
        dataLoggerARM_0.recXAxis_Z = n->getXAxis().z;
        
        dataLoggerARM_0.recYAxis_X = n->getYAxis().x;
        dataLoggerARM_0.recYAxis_Y = n->getYAxis().y;
        dataLoggerARM_0.recYAxis_Z = n->getYAxis().z;
        
        dataLoggerARM_0.recZAxis_X = n->getZAxis().x;
        dataLoggerARM_0.recZAxis_Y = n->getZAxis().y;
        dataLoggerARM_0.recZAxis_Z = n->getZAxis().z;
    }
    newRecPos_X = dataLoggerARM_0.getPositions_X;
    newRecPos_Y = dataLoggerARM_0.getPositions_Y;
    newRecPos_Z = dataLoggerARM_0.getPositions_Z;
    if (isCamMove == true) {
        if (newRecPos_X != newRecPosTest_X ||
            newRecPos_Y != newRecPosTest_Y ||
            newRecPos_Z != newRecPosTest_Z) {
            
            playbackCam_0.testMe = true;
            
            newRecPosTest_X = newRecPos_X;
            newRecPosTest_Y = newRecPos_Y;
            newRecPosTest_Z = newRecPos_Z;
        }
    }
    if (playbackCam_0.testMe == true) {
        cam[1].setPosition(playbackCam_0.smoothX, playbackCam_0.smoothY, playbackCam_0.smoothZ);
    }
    dataLoggerARM_0.update();
    //-----------------dataLoggerARM------------------_
    
    //playbackCam-------------------------------------_
    playbackCam_0.update();
    
    if (setPosAddres != setPosAddresTest){
        newPos_X = dataLoggerARM_0.getPositions_X;
        newPos_Y = dataLoggerARM_0.getPositions_Y;
        newPos_Z = dataLoggerARM_0.getPositions_Z;
        
        if (newPos_X != newPosTest_X &&
            newPos_Y != newPosTest_Y &&
            newPos_Z != newPosTest_Z) {
            newPosTest_X = newPos_X;
            newPosTest_Y = newPos_Y;
            newPosTest_Z = newPos_Z;
            playbackCam_0.to_X = dataLoggerARM_0.getPositions_X;
            playbackCam_0.to_Y = dataLoggerARM_0.getPositions_Y;
            playbackCam_0.to_Z = dataLoggerARM_0.getPositions_Z;
        }else{
            playbackCamTimer_0.topTimer.startTimer = true;
        }
        setPosAddresTest = setPosAddres;
    }
    //-----------------playbackCam--------------------_
    
    //velSmoothStateXY--------------------------------_
    velSmoothStateMouseMoved.update(mouseX, mouseY);
    //-----------------velSmoothStateXY---------------_
    
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true) {
        drHitAreaImage_0.update();
        if (drHitAreaImage_0.hitAreaImageIndex == 1) {
            isDrawOpenCV = true;
        }else{
            isDrawOpenCV = false;
        }
        if (isDrawOpenCV == true) {
            isDrHitAreaImage = true;
            bDrawPointCloud = false;
            isDrPLayHitAreaImage = false;
            dataLoggerARM_0.isKeyFill = false;
            isDataLoggerARM = false;
            dataLoggerARM_0.guiMessXML_0.recIndex_Y = dataLoggerARM_0.guiMessXML_0.maxNumInList;
            isGuiAll = true;
        }
        
        if (drHitAreaImage_0.hitAreaImageIndex == 2) {
            bDrawPointCloud = true;
            isGuiDataLoggerARM = true;
            isDrPLayHitAreaImage = false;
            isDataLoggerARM = false;
            orbitRadius = 6000;
            isGuiAll = true;
        }else{
            isGuiDataLoggerARM = false;
        }
        
        if (drHitAreaImage_0.hitAreaImageIndex == 3) {
            bDrawPointCloud = true;
            isDrHitAreaImage = true;
            isGuiDataLoggerARM = false;
            isDrPLayHitAreaImage = true;
            isGuiCamera = true;
            isDataLoggerARM = false;
            dataLoggerARM_0.isKeyFill = false;
            dataLoggerARM_0.guiMessXML_0.recIndex_Y = dataLoggerARM_0.guiMessXML_0.maxNumInList;
            isGuiAll = false;
        }else {
            isGuiCamera = false;
        }
        
        if (drHitAreaImage_0.hitAreaImageIndex == 4) {
            bDrawPointCloud = true;
            isDrHitAreaImage = false;
            isDrPLayHitAreaImage = true;
            isSession = true;
            isGuiAll = false;
            dataLoggerARM_0.isKeyFill = false;
            dataLoggerARM_0.guiMessXML_0.recIndex_Y = dataLoggerARM_0.guiMessXML_0.maxNumInList;
        }else{
            isSession = false;
        }
    }
    //-----------------drHitAreaImage-----------------_
    
    //drPLayHitAreaImage------------------------------_
//    if (isRecReset == true) {
//        isRecReset = false;
//    }
    if (isPlayReset == true) {
        camToView = 0;
        playMode = 0;
        drPLayHitAreaImage_0.isReset = true;
        
        if (recMode == 1) {
            recMode = 0;
            dataLoggerARM_0.isSetAllDefault = true;
            dataLoggerARM_0.recRecordARM_0.isGetAll = true;
            isDataLoggerARM = false;
        }
        isPlayReset = false;
    }
    drPLayHitAreaImage_0.reset();
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.update();
    }
    //-----------------drPLayHitAreaImage-------------_
    
    //timer------------_
	if (isTimerStart != isTimerStartTest) {
		//playbackCamTimer_0.topTimer.startTimer = true;
		//timer_0.topTog.startTimer = true;
        isTimerStartTest == isTimerStart;
	}
    if (playMode == 1) {
        if (playbackCam_0.testMe == false) {
            // cout << "           startTimer " << playbackCamTimer_0.topTimer.startTimer << endl;
            if (playbackCam_0.isTested == true){
                playbackCamTimer_0.topTimer.startTimer = true;
                playbackCam_0.isTested = false;
            }
//            if (isPlay == true) {
//                playbackCamTimer_0.topTimer.startTimer = true;
//                cout << "isPlay " << isPlay << endl;
//                isPlay = false;
//            }
        }
    }

	if (playbackCamTimer_0.oneStart == true){cout << "oneStart " << endl;}
	if (playbackCamTimer_0.inEnd == true){cout << "inEnd " << endl;}
	if (playbackCamTimer_0.inStart == true ){cout << "inStart " << endl;}
	if (playbackCamTimer_0.endTimer == true){
        setPosAddres++;
        if (setPosAddres >= dataLoggerARM_0.getAllPositionsXSize) {
            setPosAddres = 0;
        }
        dataLoggerARM_0.setPosAddres = setPosAddres;
//        if (playbackCam_0.testMe == true){
//            if (isPlay == false)isPlay = true;
//            cout << "isPlay " << isPlay << endl;
//        }
        cout << "setPosAddres " << setPosAddres << endl;
    }
	
    playbackCamTimer_0.thisLongTop = howLongPlaybackCam;//"This Long in Frame"
    playbackCamTimer_0.howManyTimeTop = howManyPlaybackCam;//"How Many Time Frame"
	playbackCamTimer_0.update();
	//-----timer-------_
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetColor(255, 255, 255);

    //dataLoggerARM-----------------------------------_
    if (isGuiDataLoggerARM == true) {
        glDisable(GL_CULL_FACE);
        //velSmoothStateXY--------------------------------_
        //velSmoothStateMouseMoved.draw(mouseX, mouseY);
        //-----------------velSmoothStateXY---------------_
        dataLoggerARM_0.draw();
        glEnable(GL_CULL_FACE);
    }
    //-----------------dataLoggerARM------------------_
	
    if (isDrawOpenCV == true) {
        bDrawPointCloud = false;
        glDisable(GL_CULL_FACE);
		// draw from the live kinect
		kinect.drawDepth(10, 10, 400, 300);
		kinect.draw(420, 10, 400, 300);
		
		grayImage.draw(10, 320, 400, 300);
		//contourFinder.draw(10, 320, 400, 300);
        
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        reportStream << "Rotate the point cloud with the mouse" << endl
        << "Press 'f' to switch to full screen" << endl
        << "Press 'h' to switch mirror horizontal for participant" << endl
        << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl
        << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
        << "set near threshold " << nearThreshold << " (press: + -)" << endl
        << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
        << ", fps: " << ofGetFrameRate() << endl
        << "press 'c' to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
        << "press 'UP' and 'DOWN' to change the tilt angle: " << angle << " degrees" << endl;
        ofDrawBitmapString(reportStream.str(), 425, 420);
        glEnable(GL_CULL_FACE);
    }
    
    if (isGuiDataLoggerARM == true) {
        //ofBackground(0);
        //bFullscreen
        string s = string("") +
        "\n" +
        "KEYS:\n" +
        "\n" +
        "Press 'f' to switch to full screen\n"+
        "Press 'h' to switch mirror horizontal for participant\n"+
        "\n" +
        "Out Form Field\n" +
        "\n" +
        " 'R' Makes A New Session\n" +
        " 'UP' / 'DOWN' Cycle Sessions\n" +
        " 'LEFT' / 'RIGHT' Cycle Data Logged\n" +
        " '<' , '>' or 'TAB' Cycle the Form Field\n" +
        "\n" +
        "In Form Field\n" +
        "\n" +
        " If No File Found It Will Make File After 'Enter' is Pressed.\n" +
        " 'Del', Key, 'Enter' or 'LEFT' / 'RIGHT'\n" +
        "\n" +
        "Audio\n" +
        "\n" +
        " 'p' Play\n" +
        " 'q' Paused\n" +
        "\n" +
        "Data Logging\n" +
        "\n" +
        " 1-9 Num Pad Pain Score\n" +
        " MousePressed Notable Difference\n";
        
        glDisable(GL_CULL_FACE);
        ofSetColor(255);
        //ofDisableLighting();
        ofDrawBitmapString(s, ofPoint(670, 20));
        glEnable(GL_CULL_FACE);
    }

    //ofCamera----------------------------------------_
    if (isGuiCamera == true) {
        //ofBackground(0);
        
        string s = string("") + 
        "\n" +
        "Press 'f' to switch to full screen\n"+
        "Press 'h' to switch mirror horizontal for participant\n"+
        "\n" +
        "Purple boxes (4 of them) are generic nodes with simple circular motion, linked in a hierarchy (with ofNode::setParent).\n" + 
        "Yellow boxes (2 of them) are cameras. You are looking through one of them so can only see one box on screen.\n" + 
        "\n" + 
        "KEYS:\n" + 
        "\n" + 
        "'z' reset transforms\n" + 
        "\n" + 
        "'v' switch camera to view: " + ofToString(camToView) + "\n" +
        "\n" + 
        
        "\n" + 
        "'c' switch camera to configure: " + ofToString(camToConfigure) + "\n" +
        " 't' cycle lookat\n" + 
        " 'p' cycle parent\n";
        
        //" LEFT pan left\n"+ 
        //" RIGHT pan right\n" + 
        //" UP tilt up\n" + 
        //" DOWN tilt down\n" + 
        //" , roll left\n" + 
        //" . roll right\n" + 
        //" a truck left\n" + 
        //" d truck right\n" + 
        
        //" w dolly forward\n" +
        //" s dolly backward\n" + 
        
        //" r boom up\n" +
        //" f boom down\n";
        glDisable(GL_CULL_FACE);
        ofSetColor(255);
        //ofDisableLighting();
        ofDrawBitmapString(s, ofPoint(20, 20));
        glEnable(GL_CULL_FACE);
        //ofEnableLighting();
    }
    // update camera transforms
    for(int i=0; i<kNumCameras; i++) {
        
        // lookat node if it has one
        if(lookatIndex[i] >= 0) cam[i].lookAt(testNodes[lookatIndex[i]]);
        
        // mouse orbit camera
        if(doMouseOrbit[i] && velSmoothStateMouseMoved.bStoppedX == false && velSmoothStateMouseMoved.bStoppedY == false) {// was ofGetMousePressed(0)
            static float lon = 0;
            static float lat = 0;
            
            setLon = lon = ofClamp(lon + mouseX - ofGetPreviousMouseX(), -180, 180);
            setLat = lat = ofClamp(lat + mouseY - ofGetPreviousMouseY(), -90, 90);
            
            if(lookatIndex[i] < 0) {
                cam[i].orbit(lon, lat, orbitRadius);
            } else {
                cam[i].orbit(lon, lat, orbitRadius, testNodes[lookatIndex[1]]);
            }
        }
    }

    // activate camera
    cam[camToView].begin();
    if(bDrawPointCloud == true) {
        drawPointCloud();
    }

    if (isGuiCamera == true) {
        // draw world axis
        ofDrawAxis(100);
        // draw testNodes
        for(int i=0; i<kNumTestNodes; i++) {
            ofSetColor(255, 128, 255);
            testNodes[i].draw();
        }
        
        // draw cameras
        for(int i=0; i<kNumCameras; i++) {
            ofSetColor(255, 255, 0);
            cam[i].draw();
            
            // draw line from cam to its lookat
            if(lookatIndex[i] >= 0) {
                ofSetColor(0, 255, 255);
                glBegin(GL_LINES);
                ofVec3f v1 = cam[i].getGlobalPosition();
                ofVec3f v2 = testNodes[lookatIndex[i]].getGlobalPosition();
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glEnd();
            }
            
            // draw line from cam to its parent
            if(parentIndex[i] >= 0) {
                ofSetColor(255, 255, 0);
                glBegin(GL_LINES);
                ofVec3f v1 = cam[i].getGlobalPosition();
                ofVec3f v2 = testNodes[parentIndex[i]].getGlobalPosition();
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glEnd();
            }
        }
    }
    
    // restore view to previous state (default openFrameworks view)
    cam[camToView].end();
    
    //---------------------ofCamera-------------------_
    
    
    if (isSession == true) {
        if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
            string s = string("") +
            "\n" +
            "Press 'f' to switch to full screen\n"+
            "Press 'h' to switch mirror horizontal for participant\n"+
            "\n" +
            "Configure camera:\n" +
            " 'w' dolly forward\n" +
            " 's' dolly backward\n";
            
            //" LEFT pan left\n"+
            //" RIGHT pan right\n" +
            //" UP tilt up\n" +
            //" DOWN tilt down\n" +
            //" , roll left\n" +
            //" . roll right\n" +
            //" a truck left\n" +
            //" d truck right\n" +
            
            //" r boom up\n" +
            //" f boom down\n";
            glDisable(GL_CULL_FACE);
            ofSetColor(255);
            //ofDisableLighting();
            ofDrawBitmapString(s, ofPoint(20, 20));
            glEnable(GL_CULL_FACE);
            //ofEnableLighting();
        }
    }
    
    glDisable(GL_CULL_FACE);
    ofEnableAlphaBlending();
    
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true) {
        drHitAreaImage_0.draw();
    }
    //-----------------drHitAreaImage-----------------_
    
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);

    //drPLayHitAreaImage------------------------------_
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.draw();
    }
    //-----------------drPLayHitAreaImage-------------_
    
    //timer------------_
    //playbackCamTimer_0.draw(65, 65);
    //------timer------_
    
    ofDisableAlphaBlending();
    glEnable(GL_CULL_FACE);
    
}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
	int w = 640;
	int h = 480;
    int vertexIndex = 0; 
	ofMesh mesh_0;
    ofMesh mesh_1;
    
	mesh_0.setMode(OF_PRIMITIVE_POINTS);
    mesh_1.setMode(OF_PRIMITIVE_POINTS);
	int step = 1;
    if (flipH == 0) {
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w/2; x += step) {
                int getDis = kinect.getDistanceAt(x, y);
                if(getDis > 0 && getDis < 2000) {
                    ofColor cur = kinect.getColorAt(x, y);
                    ofVec3f pos = kinect.getWorldCoordinateAt(x, y);
                    pos.x = w-pos.x;
                    
                    mesh_0.addColor(kinect.getColorAt(x, y));
                    mesh_0.addVertex(kinect.getWorldCoordinateAt(x, y));
                    
                    mesh_1.addColor(cur);
                    mesh_1.addVertex(pos);
                    
                }
            }
        }
    }
    
    if (flipH == 1) {
        for(int y = 0; y < h; y += step) {
            for(int x = w/2; x < w; x += step) {
                int getDis = kinect.getDistanceAt(x, y);
                if(getDis > 0 && getDis < 2000) {
                    ofColor cur = kinect.getColorAt(x, y);
                    ofVec3f pos = kinect.getWorldCoordinateAt(x, y);
                    pos.x = w-pos.x;
                    
                    mesh_0.addColor(kinect.getColorAt(x, y));
                    mesh_0.addVertex(kinect.getWorldCoordinateAt(x, y));
                    
                    mesh_1.addColor(cur);
                    mesh_1.addVertex(pos);
                    
                }
            }
        }
    }

	
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
    if (isScaleSessions) {
        ofScale(1, -1, -1);
    }else {
        ofScale(1, -1, -1);
    }
    ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh_0.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
    
    ofPushMatrix();
    if (isScaleSessions) {
        ofScale(1, -1, -1);
    }else {
        ofScale(1, -1, -1);
    }
    ofTranslate(-645, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh_1.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
    
    if (dataLoggerARM_0.isKeyFill == false && dataLoggerARM_0.guiMessXML_0.recIndex_Y == dataLoggerARM_0.guiMessXML_0.maxNumInList) {
        if(key == 'f'){
            
            bFullscreen = !bFullscreen;
            
            if(!bFullscreen){
                ofSetWindowShape(1224,800);
                ofSetFullscreen(false);
                ofSetWindowTitle("ARM");//"Aaugmented Reality Mirror"
                // figure out how to put the window in the center:
                ofSetWindowPosition(10, 10);
            } else if(bFullscreen == 1){
                ofSetFullscreen(true);
            }
        }
        
        if (bDrawPointCloud == true) {
            if (key == 'h' || key == 'H'){
                flipH = !flipH;
            }
        }
    
        if (isDrawOpenCV == true) {
            switch (key) {
            case ' ':
                bThreshWithOpenCV = !bThreshWithOpenCV;
                break;
            case '>':
            case '.':
                farThreshold ++;
                if (farThreshold > 255) farThreshold = 255;
                break;
                
            case '<':
            case ',':
                farThreshold --;
                if (farThreshold < 0) farThreshold = 0;
                break;
                
            case '+':
            case '=':
                nearThreshold ++;
                if (nearThreshold > 255) nearThreshold = 255;
                break;
                
            case '-':
                nearThreshold --;
                if (nearThreshold < 0) nearThreshold = 0;
                break;
                
            case 'w':
                kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
                break;
                
            case 'o':
                kinect.setCameraTiltAngle(angle); // go back to prev tilt
                kinect.open();
                break;
                
            case 'c':
                kinect.setCameraTiltAngle(0); // zero the tilt
                //kinect.close();
                break;
                
            case OF_KEY_UP:
                angle++;
                if(angle>30) angle=30;
                kinect.setCameraTiltAngle(angle);
                break;
                
            case OF_KEY_DOWN:
                angle--;
                if(angle<-30) angle=-30;
                kinect.setCameraTiltAngle(angle);
                break;
        }
            isDataLoggerARM = false;
        }
        
        ofNode *n = &cam[camToConfigure];
        if (isSession == true) {
            static float lon = 0;
            static float lat = 0;
            if (key == 'w'){
                orbitRadius -= 20;
                lon = setLon;
                lat = setLat;
                cam[0].orbit(lon, lat, orbitRadius);
            }
            if (key == 's'){
                orbitRadius += 20;
                lon = setLon;
                lat = setLat;
                cam[0].orbit(lon, lat, orbitRadius);
            }
        }
        //ofCamera----------------------------------------_
        if (isGuiCamera == true) {
            
            switch(key) {
                case 'c':
                    camToConfigure = 1 - camToConfigure;
                    printf("\n\n** MOVING CAMERA %i **\n", camToConfigure);
                    break;
                    
                case 'v':
                    camToView = 1 - camToView;
                    printf("\n\n** SHOWING CAMERA %i **\n", camToView);
                    break;
                    
                case 'z':
                    reset();
                    break;
                    
                case 't':
                    lookatIndex[camToConfigure]++ ; 
                    if(lookatIndex[camToConfigure]>=kNumTestNodes) {
                        lookatIndex[camToConfigure] = -1;
                        // cam[camToConfigure].disableTarget();
                        // } else {
                        // cam[camToConfigure].setTarget(testNodes[parentIndex[camToConfigure]]);
                    }
                    break;
                    
                case 'p':
                    parentIndex[camToConfigure]++ ; 
                    ofVec3f oldP = cam[camToConfigure].getGlobalPosition();
                    ofQuaternion oldQ = cam[camToConfigure].getGlobalOrientation();
                    if(parentIndex[camToConfigure]>=kNumTestNodes) {
                        parentIndex[camToConfigure] = -1;
                        cam[camToConfigure].clearParent();
                    } else {
                        cam[camToConfigure].setParent(testNodes[parentIndex[camToConfigure]]);
                    }
                    cam[camToConfigure].setGlobalPosition(oldP);
                    cam[camToConfigure].setGlobalOrientation(oldQ);
                    
                    break;
                
            }
        }
        //---------------------ofCamera-------------------_
    }
    
    //dataLoggerARM-----------------------------------_
    //if (isDataLoggerARM == true) {
        dataLoggerARM_0.keyPressed(key);
    //}
    if(key == ';')setPosAddres++;
    if(key == ':')setPosAddres--;
    dataLoggerARM_0.setPosAddres = setPosAddres;
    //cam[1].setPosition(dataLoggerARM_0.smoothX, dataLoggerARM_0.smoothY, dataLoggerARM_0.smoothZ);
    //-----------------dataLoggerARM------------------_
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
    if (y <= mouseMovTop_y) {
        isMouseMovTop = true;
    }else{
        isMouseMovTop = false;
    }
    if (y >= mouseMovBott_Y) {
        isMouseMovBott = true;
    }else{
        isMouseMovBott = false;
    }
    
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true)drHitAreaImage_0.mouseMoved(x, y);
    //-----------------drHitAreaImage-----------------_
    //drPLayHitAreaImage------------------------------_
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.mouseMoved(x, y);
    }
    //-----------------drPLayHitAreaImage-------------_
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true)drHitAreaImage_0.mouseDragged(x, y, button);
    //-----------------drHitAreaImage-----------------_
    //drPLayHitAreaImage------------------------------_
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.mouseDragged(x, y, button);
    }
    //-----------------drPLayHitAreaImage-------------_
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true) {
        if (drHitAreaImage_0.isHitOk == false && drPLayHitAreaImage_0.isHitOk == false) {
            dataLoggerARM_0.recRecordARM_0.isLogging = true;
        }
    }
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true){
        drHitAreaImage_0.mousePressed(x, y, button);
        if (drHitAreaImage_0.isHitOk == true) {
            if (drHitAreaImage_0.hitAreaImageIndex == 1) {
                isPlayReset = true;
            }
            if (drHitAreaImage_0.hitAreaImageIndex == 2) {
                isPlayReset = true;
            }
            if (drHitAreaImage_0.hitAreaImageIndex == 3) {
                isPlayReset = true;
            }
            if (drHitAreaImage_0.hitAreaImageIndex == 4) {
                isPlayReset = true;
                
                camToView = 0;
                //playMode = 0;
                drPLayHitAreaImage_0.menuMode = 0;
                orbitRadius = 500;
            }
        }
    }
    //-----------------drHitAreaImage-----------------_
    
    //drPLayHitAreaImage------------------------------_
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.mousePressed(x, y, button);
        if (drPLayHitAreaImage_0.isHitOk == true) {
            if (drPLayHitAreaImage_0.hitAreaImageIndex == 1) {
                //playbackCamTimer_0.topTimer.startTimer = true;
                cameraMode = 1;
                if (cameraMode == 0) {
                    camToView = 0;
                }
                if (cameraMode == 1) {
                    camToView = 1;
                }
                
                playMode = !playMode;
                if (playMode == 0) {
                    drPLayHitAreaImage_0.menuMode = 0;
                    //cout << " playMode " << playMode << endl;
                }
                if (playMode == 1) {
                    dataLoggerARM_0.isSetAllDefault = true;
                    dataLoggerARM_0.recRecordARM_0.isGetAll = true;
                    drPLayHitAreaImage_0.menuMode = 1;
                    playbackCamTimer_0.topTimer.startTimer = true;
                    //cout << " playMode " << playMode << endl;
                }
                isDataLoggerARM = false;
            }
            if (drPLayHitAreaImage_0.hitAreaImageIndex == 2) {
                cameraMode = 1;
                if (cameraMode == 0) {
                    camToView = 0;
                }
                if (cameraMode == 1) {
                    camToView = 1;
                }
                if (playMode == 0) {
                    dataLoggerARM_0.isSetAllDefault = true;
                    dataLoggerARM_0.recRecordARM_0.isGetAll = true;
                    drPLayHitAreaImage_0.menuMode = 0;
                    setPosAddres--;
                    if (setPosAddres < 0) {
                        setPosAddres = dataLoggerARM_0.getAllPositionsXSize;
                    }
                    dataLoggerARM_0.setPosAddres = setPosAddres;
                }
                if (playMode == 1) {
                    playbackCam_0.rampAmount -= rampAmount;
                    if (playbackCam_0.rampAmount <= 0.009) {
                        playbackCam_0.rampAmount = 0.009;
                        cout << rampAmount << endl;
                    }
                }
                if(isRR != isRRTest){
                    isRRTest = isRR;
                }
                isDataLoggerARM = false;
            }
            if (drPLayHitAreaImage_0.hitAreaImageIndex == 3) {
                cameraMode = 1;
                if (cameraMode == 0) {
                    camToView = 0;
                }
                if (cameraMode == 1) {
                    camToView = 1;
                }
                if (playMode == 0) {
                    dataLoggerARM_0.isSetAllDefault = true;
                    dataLoggerARM_0.recRecordARM_0.isGetAll = true;
                    drPLayHitAreaImage_0.menuMode = 0;
                    setPosAddres++;
                    if (setPosAddres >= dataLoggerARM_0.getAllPositionsXSize) {
                        setPosAddres = 0;
                    }
                    dataLoggerARM_0.setPosAddres = setPosAddres;
                }
                if (playMode == 1) {
                    playbackCam_0.rampAmount += rampAmount;
                    if (playbackCam_0.rampAmount >= 15.0) {
                        playbackCam_0.rampAmount = 15.0;
                    }
                }
                if(isFF != isFFTest){
                    isFFTest = isFF;
                }
                isDataLoggerARM = false;
            }
            if (drPLayHitAreaImage_0.hitAreaImageIndex == 4) {
                cameraMode = 0;
                if (cameraMode == 0) {
                    camToView = 0;
                }
                if (cameraMode == 1) {
                    camToView = 1;
                }
                
                playMode = 0;
                
                if(isRec != isRecTest){
                    isRecTest = isRec;
                }
                
                recMode = !recMode;
                if (recMode == 0) {
                    dataLoggerARM_0.isSetAllDefault = true;
                    dataLoggerARM_0.recRecordARM_0.isGetAll = true;
                    drPLayHitAreaImage_0.menuMode = 0;
                    isDataLoggerARM = false;
                }
                if (recMode == 1) {
                    drPLayHitAreaImage_0.menuMode = 2;
                    isDataLoggerARM = true;
                }
            }
        }
    }
    //-----------------drPLayHitAreaImage-------------_
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    //drHitAreaImage----------------------------------_
    if (isMouseMovBott == true || isMouseMovTop == true || isDrHitAreaImage == true)drHitAreaImage_0.mouseReleased(x, y, button);
    //-----------------drHitAreaImage-----------------_
    //drPLayHitAreaImage------------------------------_
    if ((isMouseMovTop == true || isMouseMovBott == true) && isDrPLayHitAreaImage == true) {
        drPLayHitAreaImage_0.mouseReleased(x, y, button);
    }
    //-----------------drPLayHitAreaImage-------------_
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
