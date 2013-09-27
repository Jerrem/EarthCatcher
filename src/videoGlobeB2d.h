//
//  videoGlobeB2d.h
//  ofxBox2dExample
//
//  Created by Jerrem Lynch on 23/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef videoGlobeB2d_h
#define videoGlobeB2d_h

#include "ofMain.h"
#include "ofxBox2d.h"
//supporting files
#include "circleMaskVideo.h"



class videoGlobeB2d : public ofxBox2dCircle {
	
public:
    bool winner;
    bool hit;
    ofImage outline;
    ofImage strike;
    circleMaskVideo Globe;
    
    // constructor
    videoGlobeB2d();
	
    
    //Methods
    void setupVid( string fileName);
    void update();
	void draw();

    //variables
    int ImageWidth;
    int ImageHeight;
};

#endif
