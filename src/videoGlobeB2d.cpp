//
//  videoGlobeB2d.cpp
//  ofxBox2dExample
//
//  Created by Jerrem Lynch on 23/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "videoGlobeB2d.h"
videoGlobeB2d::videoGlobeB2d(){
    
}

void videoGlobeB2d :: setupVid (string fileName){
    Globe.setup(fileName);
    outline.loadImage("outline.jpg");
    strike.loadImage("hit.jpg");
    ImageWidth = Globe.VideoW;
    ImageHeight = Globe.VideoH;
    winner = false;
}

void videoGlobeB2d :: update(){
    Globe.update();
}

void videoGlobeB2d::draw(){
        glPushMatrix();
        glTranslatef(getPosition().x, getPosition().y, 0);
        ofRotate(getRotation());
        ofSetColor(255, 255, 255);
        ofFill();
        Globe.draw(0,0);
        if (!hit && !winner);// outline.draw(getPosition().x - 168, getPosition().y - 168);
        else if (winner) Globe.behindGlobe.draw(getPosition().x-(640 * 1/2),getPosition().y-(480 * 1/2), 640 * 1, 480 * 1);
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        ofDisableBlendMode();
        glPopMatrix();
    }