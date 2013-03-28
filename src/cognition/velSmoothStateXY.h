//
//  Created by Lee Meredith.
//

#ifndef _VEL_SM_ST_XY
#define _VEL_SM_ST_XY


#include "ofMain.h"
#include "velSmoothState.h"

class velSmoothStateXY{
	
public:
	void setup(int newMinStoppedFrames, int newMinMovingFrames);
	void update(float newPosX, float newPosY);
	void draw(float newX, float newY);
	bool bStop();
	float velocityl();
	float accel();
	
	
	float posX;
	float posY;
	
	float velX;
	float velY;
	float accX;
	float accY;
	
	float dX;
	float dY;
	
	int minStoppedFrames;
	int minMovingFrames;
	bool bStoppedX;
	bool bStoppedY;
	int stoppedFrameCountX;
	int stoppedFrameCountY;
	int movingFrameCountX;
	int movingFrameCountY;
	
	velSmoothState stateX;
	velSmoothState stateY;
};

#endif
