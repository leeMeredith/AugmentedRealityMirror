//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "xenoRectOne.h"

class playbackCam {
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
    
    //ramp---------------------_
    float from_X, to_X;
    float from_Y, to_Y;
    float from_Z, to_Z;
    bool testMe, isTested;
    float rampTo_X, rampTo_Y, rampTo_Z, rampAmount;
    //----------ramp-----------_
    
    //xenoRectOne--------------_
    int setPosAddres;
    float catchUpSpeedX, catchUpSpeedY, catchUpSpeedZ;
    float setPosX, setPosY, setPosZ;
    float smoothX, smoothY, smoothZ;
    xenoRectOne xenoPosX, xenoPosY, xenoPosZ;
    //xenoRectOne--------------_
    
};
