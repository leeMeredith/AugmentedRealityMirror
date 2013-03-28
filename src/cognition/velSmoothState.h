//
//  Created by Lee Meredith.
//

#ifndef _VEL_SM_ST
#define _VEL_SM_ST


#include "ofMain.h"

class velSmoothState{
	
public:
	void setup(int newMinStoppedFrames, int newMinMovingFrames);
	void update(float newPos);
	void draw(float newX, float newY);
	bool bStop();
	float velocityl();
	float accel();
	
	
	float pos;
	float curr;
	float prev;
	float vel;
	float prevVel;
	float acc;
	
	float dX;
	float dY;
	
	int minStoppedFrames;
	int minMovingFrames;
	bool bStopped;
	int stoppedFrameCount;
	int movingFrameCount;
	
};

#endif
