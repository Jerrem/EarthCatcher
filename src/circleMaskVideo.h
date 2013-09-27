//
//  circleMaskVideo.h
//  this class creates a video object which is masked by a circle 
//  assumes background of the video is black, and that circle is in dead center of the video
//
//  Created by Jerrem Lynch on 29/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_circleMaskVideo_h
#define emptyExample_circleMaskVideo_h
	
#include "ofMain.h"
class circleMaskVideo : public ofVideoPlayer {
	
public:
    
    ofVideoPlayer circleMeVid;
    ofVideoPlayer behindGlobe;
    
    // constructor
    circleMaskVideo();
	
    
    //Methods
    //string imgNum (int num);
    void setup(string fileName);
    void update();
	void draw(int _x,int  _y);
    void draw(int _x,int  _y,int radius);
    
    //variables 
    float VideoW,VideoH;

};

#endif
