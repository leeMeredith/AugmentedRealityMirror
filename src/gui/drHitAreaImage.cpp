#include "drHitAreaImage.h"

//--------------------------------------------------------------
void drHitAreaImage::setup(){
    //Directory-Image-------_
    dirDialogImageStringDefault = "record/media/images/";
    dirDialogImageString = dirDialogImageStringDefault;
    setDirirDialogImageString = "bigMenu";
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
    
    //allHitAreaImage-------------_
    isAddHitDialog = true;
    allHitAreaImage_0.setup();
    allHitAreaImage_0.setPosition(11, 500);
    allHitAreaImage_0.setDimensions();
    allHitAreaImage_0.setImage(myImage);
    //-------allHitAreaImage------_
    
    currentDialogImage = 0;
    setRecordImageFolderTest = dirDialogImageString;
    //----Directory-Image---_
    
    newBoundingBoxHeight =  allHitAreaImage_0.boundingBoxHeight/2;
    width = ofGetWidth();
    height = ofGetHeight();
    setX_0 = (width/2)-(allHitAreaImage_0.allSize/2);
    cout << setX_0 << endl;
    setY_0 = newBoundingBoxHeight;// - height/2;
    setX_1 = width/2 - allHitAreaImage_0.allSize/2;
    setY_1 = height - allHitAreaImage_0.boundingBoxHeight+5;
    
    menuMode = 0;
    testMenuMode = -1;
}

//--------------------------------------------------------------
void drHitAreaImage::update(){
    width = ofGetWidth();
    height = ofGetHeight();
    newBoundingBoxHeight =  allHitAreaImage_0.boundingBoxHeight/2;
    setX_0 = (width/2)-(allHitAreaImage_0.allSize/2);
    setY_0 = height - allHitAreaImage_0.boundingBoxHeight+5;
    setX_1 = (width/2)-(allHitAreaImage_0.allSize/2);
    setY_1 = height - allHitAreaImage_0.boundingBoxHeight+5;
    
    if (allHitAreaImage_0.isSel == true) {
        menuMode = 1;
    }else {
        menuMode = 0;
    }
    
    /*
     if (menuMode == 0) {
     recImageFolderCurrentDir = 0;
     }
     if (menuMode == 1) {
     recImageFolderCurrentDir = 1;
     }
     //*/
    
    //allHitAreaImage-------------_
    allHitAreaImage_0.checkHit(mouseX, mouseY);
    allHitAreaImage_0.setDimensions();
    allHitAreaImage_0.setImage(myImage);
    isHitOk = allHitAreaImage_0.isHitOk;
    
    if (allHitAreaImage_0.isHitOk == true && isAddHitDialog == true) {
        if (menuMode == 0 && menuMode != testMenuMode) {
            //Directory-Image-------_
            dirDialogImageStringDefault = "record/media/images/";
            dirDialogImageString = dirDialogImageStringDefault;
            setDirirDialogImageString = "bigMenu";
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
            setDirirDialogImageString = "littleMenu";
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
    //-------allHitAreaImage------_
    
}

//--------------------------------------------------------------
void drHitAreaImage::draw(){
    
    //ofDirectory------------------------_
	ofSetColor(ofColor::gray);
    if (dirDialogImage.size() > 0){
		ofSetColor(ofColor::white);
		//images[currentDialogImage].draw(300,220);
        ////allHitAreaImage-------------_
        //this is bad on fps
        //for (int i = 0; i < MAX_NUM_IMAGE; i++) {
        //    if (i < MAX_NUM_IMAGE) {
        //        myImage[i] = images[i];
        //    }
        //    allHitAreaImage_0.setImage(myImage);
        //}
        ////------allHitAreaImage-------_
	}
    //-------------ofDirectory-----------_
    
    if (menuMode == 0) {
        //allHitAreaImage-------------_
        allHitAreaImage_0.draw(setX_0, setY_0);
        //-------allHitAreaImage------_
    }
    if (menuMode == 1) {
        //allHitAreaImage-------------_
        allHitAreaImage_0.draw(setX_1, setY_1);
        //-------allHitAreaImage------_
    }
    
}

//--------------------------------------------------------------
void drHitAreaImage::keyPressed(int key){

}

//--------------------------------------------------------------
void drHitAreaImage::keyReleased(int key){

}

//--------------------------------------------------------------
void drHitAreaImage::mouseMoved(int x, int y){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void drHitAreaImage::mouseDragged(int x, int y, int button){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void drHitAreaImage::mousePressed(int x, int y, int button){
    mouseX = x;
    mouseY = y;
    //allHitAreaImage----------------------_
    allHitAreaImage_0.mousePressed(x, y, button);
    isAddHitDialog = true;
    if (allHitAreaImage_0.isHitOk == true) {
        hitDialogIndex = allHitAreaImage_0.hitDialogIndex;
        hitAreaImageIndex = allHitAreaImage_0.hitAreaImageIndex;
        cout <<  hitDialogIndex <<  hitAreaImageIndex << endl;
    }
    //-----------allHitAreaImage-----------_
}

//--------------------------------------------------------------
void drHitAreaImage::mouseReleased(int x, int y, int button){
    mouseX = x;
    mouseY = y;
}