//
//  Created by Lee Meredith.
//

#pragma once

#include "ofMain.h"
#include "guiText.h"
class audioPlayer{

	public:
		void setup(int newRectX, int newRectY, string newLargeAudioDir);
		void update(int newRectX, int newRectY, string newLargeAudioDir);
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
        //xml--------------_
        ofXml getXML;
        bool isIndex;
        string setMessageDir;
        string getMessAudioXML[6];
        string audio, play, paused, volume, speed, position;
        //-------xml-------_
        
        //guiText-----_
        guiText guiText_0;
        ofTrueTypeFont knobFont;
        int rectX, rectY;
        int rectW, rectH;
        ofColor playColor, pausedColor, controlColor;
        //---guiText--_
    
        
        string largeAudioDir, largeAudioDirTest;
        bool isPlay, isPaused;
        float largeAudioPosition, largeAudioSpeed, largeAudioVolume;
        float getLargePositionMS, newGetLargePositionMS, getLargePosition, getLargeSpeed, getLargeVolume;
        ofSoundPlayer  large;
        
};

