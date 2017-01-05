#include "drPLayHitAreaImage.h"

//--------------------------------------------------------------
void drPLayHitAreaImage::setup(){
    //Directory-Image-------_
    dirDialogImageStringDefault = "record/media/images/";
    dirDialogImageString = dirDialogImageStringDefault;
    setDirirDialogImageString = "recPlaySmall_1";
    dirDialogImageString += setDirirDialogImageString;
    dirDialogImageString += "/";
    dirDialogImage.listDir(dirDialogImageString);
	dirDialogImage.sort();
    
	if(dirDialogImage.size()){
		images.assign(dirDialogImage.size(), ofImage());
	}
    
	for(int i = 0; i < (int)dirDialogImage.size(); i++){
		images[i].loadImage(dirDialogImage.getPath(i));
        if (i < MAX_NUM_IMAGE) {
            myImage[i] = images[i];
        }
	}
    
    //playbackHitAreaImage-------------_
    isAddHitDialog = true;
    playbackHitAreaImage_0.setup();
    playbackHitAreaImage_0.setPosition(11, 500);
    playbackHitAreaImage_0.setDimensions();
    playbackHitAreaImage_0.setImage(myImage);
    //-------playbackHitAreaImage------_
    
    currentDialogImage = 0;
    setRecordImageFolderTest = dirDialogImageString;
    //----Directory-Image---_
    
    newBoundingBoxHeight =  playbackHitAreaImage_0.boundingBoxHeight/2;
    width = ofGetWidth();
    height = 0;
    setX_0 = (width/2)-(playbackHitAreaImage_0.allSize/2);
    //cout << setX_0 << endl;
    setY_0 = newBoundingBoxHeight;// - height/2;
    setX_1 = width/2 - playbackHitAreaImage_0.allSize/2;
    setY_1 = height + playbackHitAreaImage_0.boundingBoxHeight+5;
    
    menuMode = 0;
    testMenuMode = -1;
}

//--------------------------------------------------------------
void drPLayHitAreaImage::update(){
    width = ofGetWidth();
    height = 0;
    newBoundingBoxHeight =  playbackHitAreaImage_0.boundingBoxHeight/2;
    setX_0 = (width/2)-(playbackHitAreaImage_0.allSize/2);
    setY_0 = height;
    setX_1 = (width/2)-(playbackHitAreaImage_0.allSize/2);
    setY_1 = height;
    
//    if (playbackHitAreaImage_0.isSel == true) {
//        menuMode = 1;
//    }else {
//        menuMode = 0;
//    }
    
     if (menuMode == 0) {
         playbackHitAreaImage_0.dialogIndex[0] = "Play";
         playbackHitAreaImage_0.dialogIndex[1] = "REW";
         playbackHitAreaImage_0.dialogIndex[2] = "FDW";
         playbackHitAreaImage_0.dialogIndex[3] = "REC";

         testSelPPMode_0.set(0, 255, 0, 255);
         testSelPPMode_1.set(255, 255, 255, 255);
         testInPPMode_0.set(0, 0, 255, 255);
         testInPPMode_1.set(0, 255, 0, 255);
         playbackHitAreaImage_0.testSelPPMode_0 = testSelPPMode_0;
         playbackHitAreaImage_0.testSelPPMode_1 = testSelPPMode_1;
         playbackHitAreaImage_0.testInPPMode_0 = testInPPMode_0;
         playbackHitAreaImage_0.testInPPMode_1 = testInPPMode_1;
         
         testSelRecMode_0.set(255, 0, 0, 255);
         testSelRecMode_1.set(255, 255, 255, 255);
         testInRecMode_0.set(255, 0, 0, 255);
         testInRecMode_1.set(50, 50, 50, 255);
         playbackHitAreaImage_0.testSelRecMode_0 = testSelRecMode_0;
         playbackHitAreaImage_0.testSelRecMode_1 = testSelRecMode_1;
         playbackHitAreaImage_0.testInRecMode_0 = testInRecMode_0;
         playbackHitAreaImage_0.testInRecMode_1 = testInRecMode_1;
     }
     if (menuMode == 1) {
         playbackHitAreaImage_0.dialogIndex[0] = "Pause";
         playbackHitAreaImage_0.dialogIndex[1] = "Slower";
         playbackHitAreaImage_0.dialogIndex[2] = "Faster";
         playbackHitAreaImage_0.dialogIndex[3] = "REC";
         testSelPPMode_0.set(255, 0, 0, 255);
         testSelPPMode_1.set(255, 255, 255, 255);
         testInPPMode_0.set(0, 255, 0, 255);
         testInPPMode_1.set(0, 0, 255, 255);
         playbackHitAreaImage_0.testSelPPMode_0 = testSelPPMode_0;
         playbackHitAreaImage_0.testSelPPMode_1 = testSelPPMode_1;
         playbackHitAreaImage_0.testInPPMode_0 = testInPPMode_0;
         playbackHitAreaImage_0.testInPPMode_1 = testInPPMode_1;
         
         testSelRecMode_0.set(255, 0, 0, 255);
         testSelRecMode_1.set(255, 255, 255, 255);
         testInRecMode_0.set(255, 0, 0, 255);
         testInRecMode_1.set(50, 50, 50, 255);
         playbackHitAreaImage_0.testSelRecMode_0 = testSelRecMode_0;
         playbackHitAreaImage_0.testSelRecMode_1 = testSelRecMode_1;
         playbackHitAreaImage_0.testInRecMode_0 = testInRecMode_0;
         playbackHitAreaImage_0.testInRecMode_1 = testInRecMode_1;
     }
    
    if (menuMode == 2) {
        playbackHitAreaImage_0.dialogIndex[0] = "Play";
        playbackHitAreaImage_0.dialogIndex[1] = "REW";
        playbackHitAreaImage_0.dialogIndex[2] = "FDW";
        playbackHitAreaImage_0.dialogIndex[3] = "Stop REC";
        testSelPPMode_0.set(255, 0, 0, 255);
        testSelPPMode_1.set(255, 255, 255, 255);
        testInPPMode_0.set(0, 255, 0, 255);
        testInPPMode_1.set(50, 50, 50, 255);
        playbackHitAreaImage_0.testSelPPMode_0 = testSelPPMode_0;
        playbackHitAreaImage_0.testSelPPMode_1 = testSelPPMode_1;
        playbackHitAreaImage_0.testInPPMode_0 = testInPPMode_0;
        playbackHitAreaImage_0.testInPPMode_1 = testInPPMode_1;
        
        testSelRecMode_0.set(255, 0, 0, 255);
        testSelRecMode_1.set(255, 255, 255, 255);
        testInRecMode_0.set(50, 50, 50, 255);
        testInRecMode_1.set(255, 0, 0, 255);
        playbackHitAreaImage_0.testSelRecMode_0 = testSelRecMode_0;
        playbackHitAreaImage_0.testSelRecMode_1 = testSelRecMode_1;
        playbackHitAreaImage_0.testInRecMode_0 = testInRecMode_0;
        playbackHitAreaImage_0.testInRecMode_1 = testInRecMode_1;
    }
    
    //playbackHitAreaImage-------------_
    playbackHitAreaImage_0.checkHit(mouseX, mouseY);
    playbackHitAreaImage_0.setDimensions();
    playbackHitAreaImage_0.setImage(myImage);
    isHitOk = playbackHitAreaImage_0.isHitOk;

    if (playbackHitAreaImage_0.isHitOk == true && isAddHitDialog == true) {
        if (menuMode == 0 && menuMode != testMenuMode) {
            
            //Directory-Image-------_
            dirDialogImageStringDefault = "record/media/images/";
            dirDialogImageString = dirDialogImageStringDefault;
            setDirirDialogImageString = "recPlaySmall_0";
            dirDialogImageString += setDirirDialogImageString;
            dirDialogImageString += "/";
            dirDialogImage.listDir(dirDialogImageString);
            dirDialogImage.sort();
            
            if(dirDialogImage.size()){
                images.assign(dirDialogImage.size(), ofImage());
            }
            
            for(int i = 0; i < (int)dirDialogImage.size(); i++){
                images[i].loadImage(dirDialogImage.getPath(i));
                if (i < MAX_NUM_IMAGE) {
                    myImage[i] = images[i];
                }
            }
        }
        if (menuMode == 1 && menuMode != testMenuMode) {
            //Directory-Image-------_
            dirDialogImageStringDefault = "record/media/images/";
            dirDialogImageString = dirDialogImageStringDefault;
            setDirirDialogImageString = "recPlaySmall_1";
            dirDialogImageString += setDirirDialogImageString;
            dirDialogImageString += "/";
            dirDialogImage.listDir(dirDialogImageString);
            dirDialogImage.sort();
            
            if(dirDialogImage.size()){
                images.assign(dirDialogImage.size(), ofImage());
            }
            
            for(int i = 0; i < (int)dirDialogImage.size(); i++){
                images[i].loadImage(dirDialogImage.getPath(i));
                if (i < MAX_NUM_IMAGE) {
                    myImage[i] = images[i];
                }
            }
        }
        if (menuMode == 2 && menuMode != testMenuMode) {
            //Directory-Image-------_
            dirDialogImageStringDefault = "record/media/images/";
            dirDialogImageString = dirDialogImageStringDefault;
            setDirirDialogImageString = "recPlaySmall_0";
            dirDialogImageString += setDirirDialogImageString;
            dirDialogImageString += "/";
            dirDialogImage.listDir(dirDialogImageString);
            dirDialogImage.sort();
            
            if(dirDialogImage.size()){
                images.assign(dirDialogImage.size(), ofImage());
            }
            
            for(int i = 0; i < (int)dirDialogImage.size(); i++){
                images[i].loadImage(dirDialogImage.getPath(i));
                if (i < MAX_NUM_IMAGE) {
                    myImage[i] = images[i];
                }
            }
        }
        testMenuMode = menuMode;
        isAddHitDialog = false;  
    }
    //-------playbackHitAreaImage------_
    
}

//--------------------------------------------------------------
void drPLayHitAreaImage::reset(){
    
    if (isReset == true) {
            menuMode = 0;
            testMenuMode = menuMode;
            //Directory-Image-------_
            dirDialogImageStringDefault = "record/media/images/";
            dirDialogImageString = dirDialogImageStringDefault;
            setDirirDialogImageString = "recPlaySmall_0";
            dirDialogImageString += setDirirDialogImageString;
            dirDialogImageString += "/";
            dirDialogImage.listDir(dirDialogImageString);
            dirDialogImage.sort();
            
            if(dirDialogImage.size()){
                images.assign(dirDialogImage.size(), ofImage());
            }
            
            for(int i = 0; i < (int)dirDialogImage.size(); i++){
                images[i].loadImage(dirDialogImage.getPath(i));
                if (i < MAX_NUM_IMAGE) {
                    myImage[i] = images[i];
                }
            }
            cout << " I DID " << endl;
        }

        isReset = false;
    }

//--------------------------------------------------------------
void drPLayHitAreaImage::draw(){
    
    //ofDirectory------------------------_
	ofSetColor(ofColor::gray);
    if (dirDialogImage.size() > 0){
		ofSetColor(ofColor::white);
		//images[currentDialogImage].draw(300,220);
        //this is bad on fps
        ////playbackHitAreaImage-------------_
        //for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        //    if (i < MAX_NUM_IMAGE) {
        //        myImage[i] = images[i];
        //    }
        //    playbackHitAreaImage_0.setImage(myImage);
        //}
        ////------playbackHitAreaImage-------_
	}
    //-------------ofDirectory-----------_
    
    if (menuMode == 0) {
        //playbackHitAreaImage-------------_
        playbackHitAreaImage_0.draw(setX_0, setY_0);
        //-------playbackHitAreaImage------_
    }
    if (menuMode == 1) {
        //playbackHitAreaImage-------------_
        playbackHitAreaImage_0.draw(setX_1, setY_1);
        //-------playbackHitAreaImage------_
    }
    if (menuMode == 2) {
        //playbackHitAreaImage-------------_
        playbackHitAreaImage_0.draw(setX_0, setY_0);
        //-------playbackHitAreaImage------_
    }
    
}

//--------------------------------------------------------------
void drPLayHitAreaImage::keyPressed(int key){

}

//--------------------------------------------------------------
void drPLayHitAreaImage::keyReleased(int key){

}

//--------------------------------------------------------------
void drPLayHitAreaImage::mouseMoved(int x, int y){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void drPLayHitAreaImage::mouseDragged(int x, int y, int button){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void drPLayHitAreaImage::mousePressed(int x, int y, int button){
    mouseX = x;
    mouseY = y;
    //playbackHitAreaImage----------------------_
    playbackHitAreaImage_0.mousePressed(x, y, button);
    isAddHitDialog = true;
    if (playbackHitAreaImage_0.isHitOk == true) {
        hitDialogIndex = playbackHitAreaImage_0.hitDialogIndex;
        hitAreaImageIndex = playbackHitAreaImage_0.hitAreaImageIndex;
        cout <<  hitDialogIndex <<  hitAreaImageIndex << endl;
    }
    //-----------playbackHitAreaImage-----------_
}

//--------------------------------------------------------------
void drPLayHitAreaImage::mouseReleased(int x, int y, int button){
    mouseX = x;
    mouseY = y;
}