//
//  testApp.h
//  EARTH CATCHER
//  Kinect based game for WWF's Earth Hour 2012 Toronto
// for details see http://jerrem.com/interactiveEvents.html
//
//  based on OpenNISample007 by gameoverhack
//  Created by Jerrem Lynch on 09/03/12.
//
//
//########################
//
// expects the following...
//
// OSX 10.6
// xcode
// openframeworks 007
// Addons 
//    openNI, ofxOpenCV, ofxBox2d, ofSyphon
//
// Media
//    bin/data/
//        win1.mov
//        win2.mov
//        win3.mov
//        lose1.mov
//        lose2.mov
//        lose3.mov
//        EarthRotates.mov
//        countdown.mov
//        overlayWin1.mov
//        overlayWin2.mov
//        WRAPPER.mov
//    bin/data/music
//            1.mp3
//            2.mp3
//            3.mp3
//            4.mp3
//            5.mp3
//    bin/data/sfx
//            1.mp3
//            2.mp3
//            3.mp3
//            4.mp3
//
//#####################

#ifndef _TEST_APP
#define _TEST_APP

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...


#include "ofMain.h"
//ofx addons
#include "ofxOpenNI.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"
#include "ofxSyphon.h"
//supporting files
#include "videoGlobeB2d.h"
#include "DrawBezPoly.h"


#define N_SOUNDS 5
#define N_VIDS 3

class objData {
public:
	int	 soundID;
	bool bHit;
    int wasHit;
    string Ident;
    string hitBy;
};

class testApp : public ofBaseApp{

public:
    //-------------------------- game mechanics and levels ##
    void setGameMech();
    void updateGameMech();
    void updateScore(); 
    void gameLevelsDraw();
    stringstream gameMsg;
    int gameLevel;
    int gameScore;
    int gameSpeed;
    int gameStartTime, gamePausedTime, gameTimeSeconds;
    bool gamePause, gameResume, isDead, isWinner;
    int hitCount;
    int maxFloorHits;
    int maxTime;
    
    //---------------------------------------- Media
    // ### Videos ###
    ofVideoPlayer newPLAYERvid;
    ofVideoPlayer earthSTARTvid;
    ofVideoPlayer earthWINNERvid[N_VIDS];
    ofVideoPlayer earthLOSERvid[N_VIDS];
    int randomVidNo;
    // ### SOUNDS ###
	ofSoundPlayer  sound[N_SOUNDS];
    ofSoundPlayer  music[N_SOUNDS];
    int randomMP3;
    
    
    // -------- ofx functions
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    void exit();
    
    //----------- openNI stuff
	void	setupRecording(string _filename = "");
	void	setupPlayback(string _filename);
	string	generateFileName();
	bool				isLive, isTracking, isRecording, isCloud, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering;
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
    ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;
    void				drawMasks();
	int					nearThreshold, farThreshold;
	int					pointCloudRotationY;
	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;
	float				filterFactor;
    
    #ifdef USE_IR
	ofxIRGenerator		recordImage, playImage;
    #else
	ofxImageGenerator	recordImage, playImage;
    #endif

    #if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
        ofxHardwareDriver	hardware;
    #endif
    
    // ------------------          opencv stuff###
    bool                depthUser;
    ofxCvGrayscaleImage users4contour;
    ofxCvContourFinder 	contourFinder;
    
    //----------------------------------------box2d STUFF ##
    void setupBox2dWorld();
    void polyToBox2d();
    void createBox2dUser();
    ofxBox2d            box2d;
    videoGlobeB2d       Earth;
    float earthBounce;
    vector      <ofxBox2dPolygon>     drawnBox2d;
	ofPolyline						drawing;		  //	we draw with this first
	ofxBox2dPolygon					polyLine;		  //	the box2d polygon/line shape
    vector<ofxBox2dPolygon>         userShapes;
    ofxBox2dPolygon                 leftWedge;
    ofxBox2dRect                    theGround;
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles

    //----->this is the function for contacts
    void contactStart(ofxBox2dContactArgs &e);
    void contactEnd(ofxBox2dContactArgs &e);
    DrawBezPoly polyDraw;
    
    //----------------------------------------syphon STUFF ##
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonClient mClient;
    
};

#endif
