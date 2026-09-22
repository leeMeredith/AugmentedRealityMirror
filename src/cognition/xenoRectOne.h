#pragma once

#include "ofMain.h"

class xenoRectOne {

	public:
	
        xenoRectOne();
	
		void	draw();
		void	xenoToOne(float catchOne);
		void	setColor(int newR, int newG, int newB);
	
		float		one;
		float		prevOne;
        float       dOne;
			
		float		catchUpSpeed;		// take this pct of where I want to be, and 1-catchUpSpeed of my pos
	
		int r0, g0, b0;
	
	
	
};

