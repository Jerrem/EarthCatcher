//
//  circleMaskVideo.cpp
//  emptyExample
//
//  Created by Jerrem Lynch on 29/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "circleMaskVideo.h"

circleMaskVideo::circleMaskVideo(){
    VideoW = 400;
    VideoH = 400;
}

void circleMaskVideo::setup(string fileName){
    circleMeVid.loadMovie(fileName);
    circleMeVid.play();
    behindGlobe.loadMovie("overlayWin1.mov");
    behindGlobe.setLoopState(OF_LOOP_NONE);
    behindGlobe.idleMovie();
    behindGlobe.stop();
    behindGlobe.setVolume(0.6);
    VideoW = circleMeVid.getWidth();
    VideoH = circleMeVid.getHeight();
}


void circleMaskVideo::update(){
    circleMeVid.update();
    behindGlobe.update();
}
void circleMaskVideo::draw(int _x,int  _y){
    ofSetColor(0, 0, 0);
    ofFill();
    ofSetCircleResolution(100);
    ofCircle(_x,_y,(VideoH/2));
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetCircleResolution(100);
    ofCircle(_x,_y,(VideoH/2));
    ofFill();  
    circleMeVid.draw(_x-(VideoW/2),_y-(VideoH/2));
}
void circleMaskVideo::draw(int _x,int  _y,int radius){
    ofSetColor(0, 0, 0);
    ofFill();
    ofSetCircleResolution(100);
    ofCircle(_x,_y,radius);
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofSetColor(255, 255, 255);
    ofFill();  
    circleMeVid.draw(_x-(VideoW/2),_y-(VideoH/2));
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
}
    