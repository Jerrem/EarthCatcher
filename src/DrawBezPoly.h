//
//  DrawBezPoly.h
//  Contact Listener Example
//
//  Created by Jerrem Lynch on 12/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifndef DrawBezPoly_h
#define DrawBezPoly_h

#include "ofxXmlSettings.h"

class DrawBezPoly  : public ofBaseApp {
	
public:
    
    //## xml stuff ####
    ofxXmlSettings XML2;
    string XmlFileName;
    void loadXML();
    void saveXML();
    int pointCount;
    int lineCount;
    int lastTagNumber;
    int tagNum;
    
    //## normal stuff ###
	DrawBezPoly();
    void setup();
    void draw();
	void drawWithEdit(); //
	void keyPressed(int key);//
	void keyReleased(int key);
	void mouseMoved(int x, int y);//
	void mouseDragged(int x, int y, int button);//
	void mousePressed(int x, int y, int button);//
	void mouseReleased(int x, int y, int button);//
    
    
    //### line drawing shiz
    bool     drawingLine, startLine, tempCurve, endingBezier;
    ofPoint                     bezStart, bez1, bez2, bezEnd;
    ofPoint                              tempLineS, tempLineE;
    ofPolyline						   drawing;
    vector  <ofPolyline>               myPolys;
};
#endif