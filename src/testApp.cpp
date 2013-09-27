//
//  testApp.cpp
//  EARTH CATCHER
//  Kinect based game for WWF's Earth Hour 2012 Toronto
// for details see http://jerrem.com/interactiveEvents.html
//
//  based on OpenNISample007 by gameoverhack
//  Created by Jerrem Lynch on 09/03/12.
//

#include "testApp.h"
int blobPoints = 0;
int Xpush = 192; //384;
int Ypush = 288; //288;
int userSCALE = 1;
bool mirrorKinect = false;
bool movingEarth = false;

//-------------------------- game mechanics and levels ##
void testApp::setGameMech(){
    Earth.setPosition(300, 30);
    gamePausedTime = 0;
    gameTimeSeconds = 0;
    gameLevel = -1;
    gameSpeed = 3; // how often things happen, number of seconds past between actions
    gamePause, gameResume = false;
    hitCount = 0;
    maxFloorHits = 1;
    randomMP3 = (ofRandom(0, N_SOUNDS));
    randomVidNo = (ofRandom(0, N_VIDS));
}
//--------------------------------------------------------------
void testApp::updateGameMech(){
    if(gameLevel==1){
        gameTimeSeconds = (ofGetElapsedTimeMicros()/1000000) - (gameStartTime + gamePausedTime); 
        if (gameTimeSeconds<-1) gameStartTime = (ofGetElapsedTimeMicros()/1000000);
        if(gameTimeSeconds>=maxTime){
            isWinner = true;
            Earth.winner = true;
        }else if(hitCount>=maxFloorHits){ 
            isDead = true;
        }
    }
}
//--------------------------------------------------------------
void testApp::updateScore(){
    if (isWinner){ 
        Earth.Globe.behindGlobe.play();
        Earth.Globe.behindGlobe.setFrame(1);
        earthWINNERvid[randomVidNo].play();
        earthWINNERvid[randomVidNo].setFrame(1);
        gameScore++;
    }
    if (isDead) {
        earthLOSERvid[randomVidNo].play();
        earthLOSERvid[randomVidNo].setFrame(1);
     gameScore--;   
    }
    isDead = false;
    isWinner = false;
}
//--------------------------------------------------------------
void testApp::gameLevelsDraw(){
    gameMsg.str("");
    objData * earthsmash = (objData*)Earth.getData();
    if (gameLevel== -1){
        Earth.hit = false;
        Earth.winner = false;
        cout<<"hear me!"<<endl;
        earthSTARTvid.setFrame(1);
        earthSTARTvid.play();
        gameMsg <<"###########"<<endl;
        gameStartTime = (ofGetElapsedTimeMicros()/1000000);
        gameLevel = 0;
    } else if (gameLevel == 0){
        gameMsg <<"COUNTDOWN to START"<<endl;
        Earth.setPosition(512,170);
        Earth.setVelocity(0, 0);
        ofSetColor(0, 0, 0);
        ofSetColor(255, 255, 255);
        earthSTARTvid.draw(0,0, 1024, 768);
        if (earthSTARTvid.getIsMovieDone()){ 
        gameLevel = 1;
        music[randomMP3].setPosition(0);
        music[randomMP3].play();
        gameStartTime = (ofGetElapsedTimeMicros()/1000000);
        }
    } else if (gameLevel == 1){
        gameMsg 
        << "poling hits"<<endl;
        Earth.hit = false;
        if (gameTimeSeconds == 10) Earth.setVelocity(0, -10);
        if(earthsmash && earthsmash->bHit) {
            cout<<"hit "<< (earthsmash->hitBy) <<endl;
            earthsmash->wasHit++;
            Earth.hit = true;
            if (movingEarth) Earth.setVelocity(0, (ofRandom(0,5)));
            if(earthsmash && earthsmash->hitBy == "GROUND"){
                hitCount++;
            }
        }
        if (isWinner || isDead) music[randomMP3].stop();
        if(isWinner) gameLevel = 2;
        else if (isDead) gameLevel = 3;
        
    } else if (gameLevel == 2){
        gameMsg << "winner"<<endl;
        Earth.setVelocity(0, 0);
        if (!Earth.Globe.behindGlobe.getIsMovieDone()){
            earthWINNERvid[randomVidNo].setFrame(1);
        }else {
            ofDisableAlphaBlending();
            ofSetColor(0, 0, 0);
            ofRect(0, 0, 1024, 768);
            ofSetColor(255, 255, 255);
            earthWINNERvid[randomVidNo].draw(0, 0, 1024, 768);
        }
        updateScore();
        if (earthWINNERvid[randomVidNo].getIsMovieDone()) setGameMech();
            earthsmash->wasHit = 0;
        
    } else if (gameLevel == 3){
        gameMsg << "looser"<<endl;
        Earth.setPosition(512,-170);
        Earth.setVelocity(0, 0);
        updateScore();
        ofSetColor(0, 0, 0);
        ofRect(0, 0, 1024, 768);
        ofSetColor(255, 255, 255);
        earthLOSERvid[randomVidNo].draw(0, 0, 1024, 768);
        if (earthLOSERvid[randomVidNo].getIsMovieDone())setGameMech();
        earthsmash->wasHit = 0;
    } else if (gameLevel == 4){
    gameMsg << "new user"<<endl;
    Earth.setPosition(512,-170);
    Earth.setVelocity(0, 0);
    updateScore();
    ofSetColor(0, 0, 0);
    ofRect(0, 0, 1024, 768);
    ofSetColor(255, 255, 255);
    newPLAYERvid.draw(0, 0, 1024, 768);
    }

}
//----------------------------------------box2d STUFF ##
void testApp::setupBox2dWorld(){}
void testApp::polyToBox2d(){
    for (int j=0; j<polyDraw.myPolys.size(); j++){
    ofxBox2dPolygon drawnPoly2Box2D;
    drawnPoly2Box2D.addVertexes(polyDraw.myPolys[j]);
    drawnPoly2Box2D.setPhysics(0.0, 0.5, 0.5);
    drawnPoly2Box2D.create(box2d.getWorld());
    drawnBox2d.push_back(drawnPoly2Box2D);
    drawnPoly2Box2D.clear();
    drawnBox2d[j].setData(new objData());
    objData * pd = (objData*)drawnBox2d[j].getData();
    pd->soundID = ofRandom(0, N_SOUNDS);
    pd->bHit	= false;
    pd->wasHit  =0;
    pd->Ident   ="line";
    pd->hitBy   ="NULL";
}}
//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		// detect collisions
        if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
            return;
        } else{
            
			objData * aData = (objData*)e.a->GetBody()->GetUserData();
			objData * bData = (objData*)e.b->GetBody()->GetUserData();
			
			if(aData) {
				aData->bHit = true;
				sound[aData->soundID].play();
                aData->wasHit++;
			}
			
			if(bData) {
				bData->bHit = true;
				sound[bData->soundID].play();
                bData->wasHit++;
			}
            
            if(aData && bData){
                aData->hitBy = bData->Ident;
                bData->hitBy = aData->Ident;
            }
		}
	}
}
//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) {
		
		objData * aData = (objData*)e.a->GetBody()->GetUserData();
		objData * bData = (objData*)e.b->GetBody()->GetUserData();
		
		if(aData) {
			aData->bHit = false;
		}
		
		if(bData) {
			bData->bHit = false;
		}
        if(aData && bData){
            aData->hitBy = "NULL";
            bData->hitBy = "NULL";
        }
	}
}
//--------------------------------------------------------------
void testApp::setup() {
    gameScore = 0;
    maxTime = 10;
    
    // kinect setup
	isLive			= true;
	isTracking		= true;
	isTrackingHands	= true;
	isFiltering		= false;
	isRecording		= false;
	isCPBkgnd		= false;
	isMasking		= true;

	nearThreshold = 500;
	farThreshold  = 1000;

	filterFactor = 0.1f;
    
    ///openNI stuff
	setupRecording();

	ofBackground(0, 0, 0);
    
    ofSetFrameRate(60);
    
    
    //syphon stuff
    ofSetWindowTitle("openNISample007Debug");
    mainOutputSyphonServer.setName("Screen Output");
    mClient.setup();
    mClient.setApplicationName("Avenue");
    mClient.setServerName("Avenue");
    
    //opencv stuff
    depthUser = false;
    users4contour.allocate(recordUser.getWidth(), recordUser.getHeight());
    cout<<ofToString(recordUser.getWidth())<<" "<<ofToString(recordUser.getHeight())<<endl;
    cout<<ofToString(recordDepth.getWidth())<<" "<<ofToString(recordDepth.getHeight())<<endl;
    
    //### box2d stuff
    box2d.init();
	box2d.setGravity(0, 10);///10);
	//box2d.createGround();
	box2d.setFPS(20.0);
	box2d.registerGrabbing();
    box2d.enableGrabbing();
    //box2d.createBounds(0,-768,1024,1516);
    //EARTH
    earthBounce = 0.5;
    Earth.setupVid("EarthRotates.mov");
    Earth.setPhysics(01.506, earthBounce, 05.0);//float density, float bounce, float friction
    Earth.setup(box2d.getWorld(), 700, 20, 168); // (b2World * b2dworld, float x, float y, float radius)
    Earth.setData(new objData());
    objData * gd = (objData*)Earth.getData();
    gd->soundID = ofRandom(0, N_SOUNDS);
    gd->bHit	= false;
    gd->wasHit  =0;
    gd->Ident   ="EARTH";
    gd->hitBy   ="NULL";

    theGround.setPhysics(0, 0.0, 0.1);
    theGround.setup(box2d.getWorld(), -500, 768, 2024, 10);
    theGround.setData(new objData());
    objData * tgd = (objData*)theGround.getData();
    tgd->soundID = ofRandom(0, N_SOUNDS);
    tgd->bHit	= false;
    tgd->wasHit  =0;
    tgd->Ident   ="GROUND";
    tgd->hitBy   ="NULL";

    	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
    	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);
	
    // load the 8 sfx soundfile
	for (int i=0; i<N_SOUNDS; i++) {
		sound[i].loadSound("sfx/"+ofToString(i)+".mp3");
		sound[i].setMultiPlay(true);
		sound[i].setLoop(false);
        sound[i].setVolume(0.1);
        music[i].loadSound("music/"+ofToString(i)+".mp3");
		music[i].setMultiPlay(true);
		music[i].setLoop(false);
	}
    
    // load and draw poly lines
    polyDraw.loadXML();
    polyToBox2d();
    
    //  start game mech
    setGameMech();
    
    // video stuff
    earthSTARTvid.setPixelFormat(OF_PIXELS_RGB);
    earthSTARTvid.loadMovie("countdown.mov");
    earthSTARTvid.setLoopState(OF_LOOP_NONE);
    earthSTARTvid.idleMovie();
    earthSTARTvid.stop();
    newPLAYERvid.setPixelFormat(OF_PIXELS_RGB);
    newPLAYERvid.loadMovie("waiting.mov");
    newPLAYERvid.setLoopState(OF_LOOP_PALINDROME);
    newPLAYERvid.idleMovie();
    newPLAYERvid.stop();
    
    	for (int i=0; i<N_VIDS; i++) {
    earthWINNERvid[i].setPixelFormat(OF_PIXELS_RGB);
            //earthWINNERvid[i].loadMovie("WRAPPER.mov");
            if (i == 0) earthWINNERvid[i].loadMovie("win1.mov");
            if (i == 1) earthWINNERvid[i].loadMovie("win2.mov");
            if (i == 2) earthWINNERvid[i].loadMovie("win3.mov");
    
    earthWINNERvid[i].setLoopState(OF_LOOP_NONE);
    earthWINNERvid[i].idleMovie();
    earthWINNERvid[i].stop();
    earthLOSERvid[i].setPixelFormat(OF_PIXELS_RGB);
    if (i == 0) earthLOSERvid[i].loadMovie("lose1.mov");
    if (i == 1) earthLOSERvid[i].loadMovie("lose2.mov");
    if (i == 2) earthLOSERvid[i].loadMovie("lose3.mov");
    earthLOSERvid[i].setLoopState(OF_LOOP_NONE);
    earthLOSERvid[i].idleMovie();
    earthLOSERvid[i].stop();
        }
}

void testApp::setupRecording(string _filename) {

#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
	hardware.setup();				// libusb direct control of motor, LED and accelerometers
	hardware.setLedOption(LED_OFF); // turn off the led just for yacks (or for live installation/performances ;-)
#endif

	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);

	recordUser.setup(&recordContext);
	recordUser.setMaxNumberOfUsers(2);					// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)

	recordContext.toggleRegisterViewport();
	if (mirrorKinect) recordContext.toggleMirror();

}

void testApp::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);
	playImage.setup(&playContext);

	playUser.setup(&playContext);
	playUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
	playUser.setUseMaskPixels(isMasking);
	playUser.setUseCloudPoints(isCloud);

	playHandTracker.setup(&playContext, 4);
	playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
	playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	playContext.toggleRegisterViewport();
    
    
	//playContext.toggleMirror();
    


}
//--------------------------------------------------------------
void testApp:: drawMasks() {
	glPushMatrix();
    recordImage.draw(Xpush, Ypush, 640, 480);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	if (depthUser) depthRangeMask.draw(Xpush,Ypush, 640, 480);
    else allUserMasks.draw(Xpush,Ypush, 640, 480);
	glDisable(GL_BLEND);
    glPopMatrix();

	
}

//--------------------------------------------------------------

void testApp::createBox2dUser(){
    
    if (userShapes.size()>=1){
        for (int u=userShapes.size(); u>0; u--){
            userShapes[u-1].destroy();
            userShapes.pop_back();
        }
    }
if ((int)contourFinder.blobs.size()>=1){
    for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
		ofNoFill();
		ofBeginShape();
        ofPolyline tempUserShape;
        
		for( int j=0; j<contourFinder.blobs[i].nPts; j++ ) {
            tempUserShape.addVertex(Xpush + contourFinder.blobs[i].pts[j].x * userSCALE, Ypush + contourFinder.blobs[i].pts[j].y * userSCALE);
        }
		//
        
        ofEndShape();
        tempUserShape.addVertex(Xpush + contourFinder.blobs[i].pts[0].x * userSCALE, Ypush + contourFinder.blobs[i].pts[0].y * userSCALE);
        tempUserShape.close();
        tempUserShape.simplify(0.7);
        ofxBox2dPolygon tempUserShapeADD;
        tempUserShapeADD.addVertexes(tempUserShape);
        tempUserShape.clear();
        tempUserShapeADD.setPhysics(0, 0,0.2);
        tempUserShapeADD.create(box2d.getWorld());
        userShapes.push_back(tempUserShapeADD);
        
        userShapes[i].setData(new objData());
        objData * ud = (objData*)userShapes[i].getData();
        ud->soundID = ofRandom(0, N_SOUNDS);
        ud->bHit	= false;
        ud->wasHit  =0;
        ud->Ident   ="User";
        ud->hitBy   ="NULL";
        
        tempUserShapeADD.clear();
        
	}
}
}

//##############################################################

void testApp::update(){
    
                                                            ///// Box2D updates
    updateGameMech();
    createBox2dUser();
    Earth.update();
    if (gameLevel == 1){
    box2d.update();
    }
    earthSTARTvid.update();
    for (int i=0; i<N_VIDS; i++) {
    earthWINNERvid[i].update();
    earthLOSERvid[i].update();
    }
    newPLAYERvid.update();
    
    
                                                            /////  openNI stuff
#ifdef TARGET_OSX // only working on Mac at the moment
	hardware.update();
#endif

	if (isLive) {
        
		// update all nodes
		recordContext.update();
		recordDepth.update();
		recordImage.update();

		// demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),recordDepth.getWidth(), recordDepth.getHeight(), OF_IMAGE_GRAYSCALE);
        
        //opencv contour finding stuff...
        if (depthUser){
            users4contour.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),recordDepth.getWidth(), recordDepth.getHeight());
        }else{
        users4contour.setFromPixels(recordUser.getUserPixels(), recordUser.getWidth(), recordUser.getHeight());
        }
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        users4contour.blurGaussian(2);
contourFinder.findContours(users4contour, 20, (640*480)/4, 5, true);	// find holes
    //

		// update tracking/recording nodes
		if (isTracking) recordUser.update();
		//if (isRecording) oniRecorder.update();

		// demo getting pixels from user gen
		if (isTracking && isMasking) {
			allUserMasks.setFromPixels(recordUser.getUserPixels(), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(recordUser.getUserPixels(1), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(recordUser.getUserPixels(2), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
		}


	} else {

		// update all nodes
		playContext.update();
		playDepth.update();
		
        //opencv contour finding stuff...
        users4contour.setFromPixels(playUser.getUserPixels(), playUser.getWidth(), playUser.getHeight());
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        users4contour.blurGaussian(2);
        contourFinder.findContours(users4contour, 20, (640*480)/4, 5, true);	// find holes
        //
        
		// update tracking/recording nodes
		if (isTracking) playUser.update();

		// demo getting pixels from user gen
		if (isTracking && isMasking) {
			allUserMasks.setFromPixels(playUser.getUserPixels(), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(playUser.getUserPixels(1), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(playUser.getUserPixels(2), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
		}
	}
    

}

//----------------------------------------------------------------------------------------------------       ____
//####################################################################################################       DRAW

void testApp::draw(){
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMasks();
	ofSetColor(255, 255, 255);
	glPushMatrix();
    ofSetColor(55, 55, 55);
    ofFill();
    ofRect(Xpush, Ypush + 472, 640, 5);
    ofSetColor(255, 255, 255);

    for (int u=0; u < userShapes.size(); u++){
        ofFill();
        userShapes[u].draw();
        ofNoFill();
    }
    
    Earth.draw();
    
    polyDraw.drawWithEdit();
    if (drawnBox2d.size()>=1){
    for(int i=0; i<drawnBox2d.size(); i++) {
        //ofNoFill();
        ofSetColor(100, 150, 100);
        polyDraw.myPolys[i].draw();
        ofSetColor(40, 40, 40);
        drawnBox2d[i].draw();
        
    }
    }

    
    gameLevelsDraw();
    
    
    
	ofSetColor(255, 255, 0);

	string statusPlay		= (string)(isLive ? "LIVE STREAM" : "PLAY STREAM");
	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	string statusSkeleton	= (string)(isTracking ? "TRACKING USERS: " + (string)(isLive ? ofToString(recordUser.getNumberOfTrackedUsers()) : ofToString(playUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
	string statusSmoothSkel = (string)(isLive ? ofToString(recordUser.getSmoothing()) : ofToString(playUser.getSmoothing()));
	string statusHands		= (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING");
	string statusFilter		= (string)(isFiltering ? "FILTERING" : "NOT FILTERING");
	string statusFilterLvl	= ofToString(filterFactor);
	string statusSmoothHand = (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));
	string statusMask		= (string)(!isMasking ? "HIDE" : (isTracking ? "SHOW" : "YOU NEED TO TURN ON TRACKING!!"));
	//string statusCloud		= (string)(isCloud ? "ON" : "OFF");
	string statusCloudData	= (string)(isCPBkgnd ? "SHOW BACKGROUND" : (isTracking ? "SHOW USER" : "YOU NEED TO TURN ON TRACKING!!"));

	string statusHardware;

#ifdef TARGET_OSX // only working on Mac at the moment
	ofPoint statusAccelerometers = hardware.getAccelerometers();
	stringstream	statusHardwareStream;

	statusHardwareStream
	<< "ACCELEROMETERS:"
	<< " TILT: " << hardware.getTiltAngle() << "/" << hardware.tilt_angle
	<< " x - " << statusAccelerometers.x
	<< " y - " << statusAccelerometers.y
	<< " z - " << statusAccelerometers.z;

	statusHardware = statusHardwareStream.str();
#endif

    
	stringstream msg;
    
	msg
	<< "    s : start/stop recording  : " << statusRec << endl
	<< "    p : playback/live streams : " << statusPlay << endl
	<< "    t : skeleton tracking     : " << statusSkeleton << endl
	<< "( / ) : smooth skely (openni) : " << statusSmoothSkel << endl
	<< "    h : hand tracking         : " << statusHands << endl
	<< "    f : filter hands (custom) : " << statusFilter << endl
	<< "[ / ] : filter hands factor   : " << statusFilterLvl << endl
	<< "; / ' : smooth hands (openni) : " << statusSmoothHand << endl
	<< "    m : drawing masks         : " << statusMask << endl
	//<< "    c : draw cloud points     : " << statusCloud << endl
	<< "    b : cloud user data       : " << statusCloudData << endl
	<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
	<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() <<"   -  "<< ofToString(blobPoints)<< endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) <<endl;// "  " << statusHardware <<endl;
    
    
    // Syphon Stuff
    ofSetColor(255, 255, 255);
    ofEnableAlphaBlending();    
    mainOutputSyphonServer.publishScreen();
    
//    glPushMatrix();
//    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
//    ofSetColor(80, 80, 80);
//    if (isLive){ recordDepth.draw(Xpush,Ypush,640,480);}
//    else {playDepth.draw(Xpush,Ypush,640,480);}
//    glPopMatrix();
    //drawMasks();
//	ofSetColor(255, 255, 255);
//    mClient.draw(50, 50);
//    ofDrawBitmapString(msg.str(), 20, 560);
//    ofSetColor(50, 255, 255);
//    gameMsg
//    <<"score "<< ofToString(gameScore)<<endl
//    <<"hitCount "<<ofToString(hitCount)<<endl
//    <<"Game Time "<<ofToString(gameTimeSeconds)<<" / "<< ofToString(maxTime)<<endl
//    <<"gamestarttime "<<ofToString(gameStartTime)<<endl
//    <<"OF time "<<ofToString((ofGetElapsedTimeMicros()/1000000))<<endl
//    <<"---------------------"<<endl
//    <<"DepthUser "<<ofToString(depthUser)<<endl
//    <<"k for delete line, l to create a line"<<endl
//    <<"7&8 - earth bounce rate"<<ofToString(earthBounce)<<endl;
//    
//    ofDrawBitmapString(gameMsg.str(), 20, 50);
    
        }

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	float smooth;
    
    if (key == '1'){
        maxTime--;
    }
    if (key == '2'){
        maxTime++;
    }
    
    if (key == '5'){
        ofToggleFullscreen();
    }
    
    if (key == '#'){
        if (depthUser) depthUser = false;
        else depthUser = true;
    }
    
    if (key == '@'){
        if (gameLevel==4){ 
            newPLAYERvid.stop();
            gameLevel = -1;
        }else{ 
            music[randomMP3].stop();
            newPLAYERvid.setFrame(1);
            newPLAYERvid.play();
            gameLevel = 4;
        }
    }

    if(key == OF_KEY_RIGHT) {
        //isEarth == true;
        polyToBox2d();
        Earth.setPosition(512,20);
        Earth.setVelocity(0, 0);
	}
    if(key == 'k'){
        if(drawnBox2d.size()>=1) {
            drawnBox2d[drawnBox2d.size()-1].destroy();
            drawnBox2d.pop_back();
        }
        if(polyDraw.myPolys.size()>=1) {
            polyDraw.myPolys.pop_back();
            polyDraw.lastTagNumber--;
            polyDraw.XML2.removeTag("SHAPE", polyDraw.lastTagNumber);
        }}
        
    polyDraw.keyPressed(key);

	switch (key) {
#ifdef TARGET_OSX // only working on Mac at the moment
		case 357: // up key
			hardware.setTiltAngle(hardware.tilt_angle++);
			break;
		case 359: // down key
			hardware.setTiltAngle(hardware.tilt_angle--);
			break;
#endif
		case 's':
		case 'S':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				break;
			} else {
				oniRecorder.startRecord(generateFileName());
				isRecording = true;
				break;
			}
			break;
		case 'p':
		case 'P':
			if (oniRecorder.getCurrentFileName() != "" && !isRecording && isLive) {
				setupPlayback(oniRecorder.getCurrentFileName());
				isLive = false;
			} else {
				isLive = true;
			}
			break;
		case 't':
		case 'T':
			isTracking = !isTracking;
			break;

		case '>':
		case '.':
			farThreshold += 50;
			if (farThreshold > recordDepth.getMaxDepth()) farThreshold = recordDepth.getMaxDepth();
			break;
		case '<':
		case ',':
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;
		case '&':
		case '7':
			earthBounce = earthBounce - 0.1;
            Earth.setPhysics(01.506, earthBounce, 05.0);
			break;
            
		case '*':
		case '8':
			earthBounce = earthBounce + 0.1;
            Earth.setPhysics(01.506, earthBounce, 05.0);
			break;
		case '+':
		case '=':
			nearThreshold += 50;
			if (nearThreshold > recordDepth.getMaxDepth()) nearThreshold = recordDepth.getMaxDepth();
			break;

		case '-':
		case '_':
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			recordContext.toggleRegisterViewport();
			break;
		default:
			break;
	}
}

string testApp::generateFileName() {

	string _root = "kinectRecord";

	string _timestamp = ofToString(ofGetDay()) +
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());

	string _filename = (_root + _timestamp + ".oni");

	return _filename;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if (isCloud) pointCloudRotationY = x;
    polyDraw.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
polyDraw.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//    if (circles.size()<10){
//		float r = ofRandom(4, 20);		// a random radius 4px - 20px
//		ofxBox2dCircle circle;
//		circle.setPhysics(3.0, 0.53, 0.1);
//		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
//        //inserting data associated with this object!! yay
//		circle.setData(new objData());
//		objData * sd = (objData*)circle.getData();
//		sd->soundID = ofRandom(0, N_SOUNDS);
//		sd->bHit	= false;
//        sd->wasHit  =0;
//        sd->Ident   ="circle";
//        sd->hitBy   ="NULL";
//        
//        circles.push_back(circle);
//    }
    polyDraw.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    polyDraw.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void testApp::exit(){
polyDraw.saveXML();
}

