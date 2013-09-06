//
//  Created by Lee Meredith.
//

#include "playbackCam.h"

//--------------------------------------------------------------
void playbackCam::setup() {
    
    //ramp---------------------_
    from_X = to_X = 0;
    from_Y = to_Y = 0;
    from_Z = to_Z = 0;
    
    testMe = isTested = false;
    
    rampTo_X = rampTo_Y = rampTo_Z = 0; 
    rampAmount = 0.1;
    //-----------ramp----------_
    
    //xenoRectOne--------------_
    catchUpSpeedX = catchUpSpeedY = catchUpSpeedZ = 0.9f;
    xenoPosX.catchUpSpeed = catchUpSpeedX;
    xenoPosY.catchUpSpeed = catchUpSpeedY;
    xenoPosZ.catchUpSpeed = catchUpSpeedZ;
    smoothX = smoothY = smoothZ = 0;
    setPosAddres = 0;
    //xenoRectOne--------------_
}

//--------------------------------------------------------------
void playbackCam::update() {
    
    //ramp---------------------_
    if (testMe == true) {
		if (to_X != from_X) {
            
			if (ofGetFrameNum() > 0){
                rampTo_X += rampAmount;
			}
            
            if (to_X < from_X) {
                from_X -= rampTo_X;
                if (from_X <= to_X) {
                    rampTo_X = 0;
                    from_X = to_X;
                }
            }
            
            if (to_X > from_X) {
                from_X += rampTo_X;
                if (from_X >= to_X) {
                    rampTo_X = 0;
                    from_X = to_X;
                }
            }
            //cout << " playbackCam.from_X " << from_X << endl;
		}
        
        if (to_Y != from_Y) {
            
			if (ofGetFrameNum() > 0){
                rampTo_Y += rampAmount;
			}
            
            if (to_Y < from_Y) {
                from_Y -= rampTo_Y;
                if (from_Y <= to_Y) {
                    rampTo_Y = 0;
                    from_Y = to_Y;
                }
            }
            
            if (to_Y > from_Y) {
                from_Y += rampTo_Y;
                if (from_Y >= to_Y) {
                    rampTo_Y = 0;
                    from_Y = to_Y;
                }
            }
            //cout << " playbackCam.from_Y " << from_Y << endl;
		}

        if (to_Z != from_Z) {
            
			if (ofGetFrameNum() > 0){
                rampTo_Z += rampAmount;
			}
            
            if (to_Z < from_Z) {
                from_Z -= rampTo_Z;
                if (from_Z <= to_Z) {
                    rampTo_Z = 0;
                    from_Z = to_Z;
                }
            }
            
            if (to_Z > from_Z) {
                from_Z += rampTo_Z;
                if (from_Z >= to_Z) {
                    rampTo_Z = 0;
                    from_Z = to_Z;
                }
            }
            //cout << " playbackCam.from_Z " << from_Z << endl;
		}
        
        if (from_X == to_X &&
            from_Y == to_Y &&
            from_Z == to_Z) {
            
            if (testMe == true) {
                isTested = true;
            }
            testMe = false;
        }
        //cout << " playbackCam.testMe " << testMe << endl;
    }
    //-----------ramp----------_
    
    //xenoRectOne--------------_
    xenoPosX.catchUpSpeed = catchUpSpeedX;
    xenoPosY.catchUpSpeed = catchUpSpeedY;
    xenoPosZ.catchUpSpeed = catchUpSpeedZ;
    xenoPosX.xenoToOne(from_X);
    xenoPosY.xenoToOne(from_Y);
    xenoPosZ.xenoToOne(from_Z);
    
    smoothX = xenoPosX.one;//or xenoPosX.prevOne ??
    smoothY = xenoPosY.one;
    smoothZ = xenoPosZ.one;
    //xenoRectOne--------------_
}

//--------------------------------------------------------------
void playbackCam::draw() {

}

//--------------------------------------------------------------
void playbackCam::exit() {

}

//--------------------------------------------------------------
void playbackCam::keyPressed (int key) {
    
}

//--------------------------------------------------------------
void playbackCam::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void playbackCam::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void playbackCam::mousePressed(int x, int y, int button){
    
}
