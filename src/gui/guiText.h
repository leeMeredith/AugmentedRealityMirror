//
//  Created by Lee Meredith.
//

#pragma once

#include "ofTrueTypeFont.h"
#include <string>

class guiText {
	
public:
	void setup();
	void setFont(ofTrueTypeFont fontPtr);
	float getTextWidth();
	float getTextHeight();
	float getTextSingleLineHeight();
	void setText(std::string textStr);
	void renderText(float x, float y);
	void renderString(std::string textToRender, float x, float y);
	float stringHeight(std::string textToRender);
	float stringWidth(std::string textToRender);
	
	ofTrueTypeFont ourFont;
	
	std::string textString;
	
	int numNewLines;
	int fontSize;
	bool bRealFont;
	bool bShowText;
    
    ofRectangle bounds;
};
