//
//  Created by Lee Meredith.
//

#pragma once

#include "ofTrueTypeFont.h"

class guiText {
	
public:
	void setup();
	void setFont(ofTrueTypeFont fontPtr);
	float getTextWidth();
	float getTextHeight();
	float getTextSingleLineHeight();
	void setText(string textStr);
	void renderText(float x, float y);
	void renderString(string textToRender, float x, float y);
	float stringHeight(string textToRender);
	float stringWidth(string textToRender);
	
	ofTrueTypeFont ourFont;
	
	string textString;
	
	int numNewLines;
	int fontSize;
	bool bRealFont;
	bool bShowText;
    
    ofRectangle bounds;
};
