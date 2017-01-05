/*
 *  allHitAreaImage.cpp
 */

#include "allHitAreaImage.h"

static bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}

//-------------------------------------------
bool allHitAreaImage::checkHit(float new_x, float new_y){
    float x = new_x;
    float y = new_y;
    
	for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        hitAreaImage_0[i].checkHit(x, y);
        isIn[i] = hitAreaImage_0[i].isIn;
        if (hitAreaImage_0[i].isIn == true) {
            isHitOk = true;
            hitAreaImageIndex = i+1;
            hitDialogIndex = dialogIndex[i];
        }
    }
    if (hitAreaImage_0[0].isIn == false &&
        hitAreaImage_0[1].isIn == false &&
        hitAreaImage_0[2].isIn == false &&
        hitAreaImage_0[3].isIn == false){
        isHitOk = false;
    }
}

//--------------------------------------------------------------
void allHitAreaImage::setup(){
    pressedSel_x = hitAreaImage_0[0].boundingBox.getX();
    pressedSel_y = hitAreaImage_0[0].boundingBox.getY();
    isSel = false;
    
    testSelPPMode_0.set(255, 0, 0, 255);
    testSelPPMode_1.set(0, 0, 0, 75);
    testInPPMode_0.set(255, 0, 0, 255);
    testInPPMode_1.set(255, 255, 255, 255);
    testSelRecMode_0.set(255, 0, 0, 255);
    testSelRecMode_1.set(255, 0, 0, 255);
    testInRecMode_0.set(255, 0, 0, 255);
    testInRecMode_1.set(255, 0, 0, 255);
    
    hitAreaImage_0[0].setColorSel_0 = testSelPPMode_0;
    hitAreaImage_0[0].setColorSel_1 = testSelPPMode_1;
    hitAreaImage_0[0].setColorIn_0 = testInPPMode_0;
    hitAreaImage_0[0].setColorIn_1 = testInPPMode_1;
    
    hitAreaImage_0[1].setColorSel_0 = testSelPPMode_0;
    hitAreaImage_0[1].setColorSel_1 = testSelPPMode_1;
    hitAreaImage_0[1].setColorIn_0 = testInPPMode_0;
    hitAreaImage_0[1].setColorIn_1 = testInPPMode_1;
    
    hitAreaImage_0[2].setColorSel_0 = testSelPPMode_0;
    hitAreaImage_0[2].setColorSel_1 = testSelPPMode_1;
    hitAreaImage_0[2].setColorIn_0 = testInPPMode_0;
    hitAreaImage_0[2].setColorIn_1 = testInPPMode_1;
    
    hitAreaImage_0[3].setColorSel_0 = testSelPPMode_0;
    hitAreaImage_0[3].setColorSel_1 = testSelPPMode_1;
    hitAreaImage_0[3].setColorIn_0 = testInPPMode_0;
    hitAreaImage_0[3].setColorIn_1 = testInPPMode_1;
    
    //guiText-----_
    myFont.loadFont("mono.ttf", 16, false);
	guiText_0.setup();
	guiText_0.setFont(myFont);
	guiText_0.setText(dialogIndex[3]);
	//rectW = guiText_0.getTextWidth();
	//rectH = guiText_0.getTextHeight();
    //--guiText---_

}
//-----------------------------------------------
void allHitAreaImage::setPosition(float new_x, float new_y){
    float x = new_x;
    float y = new_y;
    isImageWidthNew = true;
    testAddPosX = 0;
    dialogIndex[0] =  "SENSOR";
    dialogIndex[1] =  "FORM";
    dialogIndex[2] =  "CAMERA";
    dialogIndex[3] =  "SESSION";
    
    for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        hitAreaImage_0[i].setPosition(x, y);
    }
}

//------------------------------------------------
void allHitAreaImage::setDimensions(){
    for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        hitAreaImage_0[i].setDimensions();
    }
}

//--------------------------------------------------------------
void allHitAreaImage::setImage(ofImage newImage[MAX_NUM_IMAGE]){
    for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        hitAreaImage_0[i].setImage(newImage[i]);
    }
}

//--------------------------------------------------------------
void allHitAreaImage::draw(float new_x, float new_y){
    float x = new_x;
    float y = new_y;
    
    if (isImageWidthNew == true){
        testAddPosX = 0;
        for (int i = 0; i < MAX_NUM_IMAGE; i++) {
            if (hitAreaImage_0[i].boundingBox.getWidth() > testAddPosX){
                boundingBoxWidth = hitAreaImage_0[i].boundingBox.getWidth();
                boundingBoxHeight = hitAreaImage_0[i].boundingBox.getHeight();
                testAddPosX = hitAreaImage_0[i].boundingBox.getWidth();
                addPosX = hitAreaImage_0[i].boundingBox.getWidth()+5;
            }
        }
        //isImageWidthNew = false;
    }
    allSize = addPosX*6;
    //addPosX = hitAreaImage_0[0].boundingBox.getWidth()+5;
    for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        x += addPosX;
        //cout << " " << x << " " << addPosX << endl;
        hitAreaImage_0[i].draw(x, y);
        if (hitAreaImage_0[i].isIn == true) {
            //--guiText---_
            float setX_1, setY_1;
            setX_1 = x + (hitAreaImage_0[i].boundingBox.getWidth()/2)-(guiText_0.stringWidth(dialogIndex[i])/2);
            //setY_1 = height - allHitAreaImage_0.boundingBoxHeight+5;
            
            guiText_0.renderString(dialogIndex[i], setX_1, y-3);
            //--guiText---_
        }
    }
}

//--------------------------------------------------------------
void allHitAreaImage::mousePressed(int x, int y, int button){
    for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        if (hitAreaImage_0[i].isIn == true){
            hitAreaImage_0[i].isSel = true;
            if (hitAreaImageIndex == 4) {
                isSel = true;
            }else {
                isSel = false;
            }

        }else {
            hitAreaImage_0[i].isSel = false;
        }
    }
}