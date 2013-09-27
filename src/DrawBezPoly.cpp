//
//  DrawBezPoly.cpp
//  Contact Listener Example
//
//  Created by Jerrem Lynch on 12/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//##############################################################
//
//        CONTAINS BEZIER CURVE DRAWING (via right button drag)
//
//##############################################################
#include <iostream>

#include "DrawBezPoly.h"

DrawBezPoly::DrawBezPoly(){
    endingBezier = false;
    bezStart.set(0,0);
    bez1.set(0,0);
    bez2.set(0,0);
    bezEnd.set(0,0);
    tempLineS.set(0,0);
    tempLineE.set(0,0);
    startLine= true;
    drawingLine = false;
    XmlFileName = "savedPolys";
    lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
    tagNum          = 0;
    
}
 //-----------------------------------------------------------XML
 void DrawBezPoly::loadXML(){
 if( XML2.loadFile(XmlFileName+".xml") ){
     int numSHAPES = XML2.getNumTags("SHAPE");
     if(numSHAPES > 0){
         for (int i=0; i<numSHAPES; i++){
             XML2.pushTag("SHAPE", i);
             lastTagNumber = i;
             int numPtTags = XML2.getNumTags("PT");
             for(int i = 0; i < numPtTags; i++){
                 string tagType = XML2.getAttribute("PT", "type:", "NULL", i);
                 
                 if(tagType == "addVertex"){
                     int x = XML2.getValue("PT:X", 0, i);
                     int y = XML2.getValue("PT:Y", 0, i);
                     drawing.addVertex(x, y);
                     cout<<"point loaded"<<endl;
                 } else if(tagType == "bezierTo"){
                    int cx1 = XML2.getValue("PT:CX1",0, i);
                    int cy1 =  XML2.getValue("PT:CY1",0, i);
                    int cx2 =  XML2.getValue("PT:CX2",0, i);
                    int cy2 =  XML2.getValue("PT:CY2",0, i);
                    int ex =  XML2.getValue("PT:X",0, i);
                    int ey = XML2.getValue("PT:Y",0, i);
                    drawing.bezierTo(cx1, cy1, cx2, cy2, ex, ey);
                     cout<<"bez load"<<endl;
                 }
             }
           
               
             if (numPtTags>=3) myPolys.push_back(drawing);
             XML2.popTag();
             drawing.clear();
             
         }
         lastTagNumber++;
         
     }
 cout<<"savedPolys.xml loaded!"<<endl;
 }else{
 cout<<"unable to load savedPolys.xml, try pressing d to set defaults."<<endl;
 }
 }
 //-----------------------------------------------------------XML
 void DrawBezPoly::saveXML(){
     XML2.saveFile(XmlFileName+".xml");
     XML2.saveFile("backups/"+XmlFileName+ofToString(ofGetTimestampString())+"BU.xml");
     cout<<"POLYS saved to xml!"<<endl;
 }
//--------------------------------------------------------------
void DrawBezPoly::setup(){
}
//--------------------------------------------------------------
void DrawBezPoly::draw(){
}
//--------------------------------------------------------------
void DrawBezPoly::drawWithEdit(){
    
    if (drawingLine){
        ofSetColor(255, 0, 0);
        ofFill();
        ofCircle(10, 10, 10);
        ofNoFill();
        drawing.draw();
        if (tempCurve){//temp curve
            ofSetColor(100, 0, 100);
            ofPolyline temp;
            temp.addVertex(bezStart);
            temp.bezierTo(bez1, bez2, bezEnd);
            temp.draw();
        }else{   //temp line
            ofSetColor(100, 0, 100);
            ofLine(tempLineS, tempLineE);
        }
        ofSetColor(0, 255, 0);
        ofLine(bezStart, bez1);  //temp outgoing Bezier
        ofSetColor(0, 0, 255);
        if (endingBezier) ofLine(bez2, bezEnd); //temp incoming Bezier
        ofFill();
        
	}
}
//--------------------------------------------------------------
void DrawBezPoly::keyPressed(int key){
    if(key == 'l'){
        if (!drawingLine){
            drawingLine = true;
            endingBezier = false;
            startLine = true;
            lastTagNumber = XML2.addTag("SHAPE");
            XML2.pushTag("SHAPE", lastTagNumber);
        }else{
            drawingLine = false;
            if(drawing.size()>=3){
                drawing.setClosed(false);
                myPolys.push_back(drawing);
                lastTagNumber++;
            }
            XML2.popTag();    
            drawing.clear();
        }
    }
}
//--------------------------------------------------------------
void DrawBezPoly::keyReleased(int key){}
//--------------------------------------------------------------
void DrawBezPoly::mouseMoved(int x, int y){
	if (drawingLine){
        tempLineE.set(x, y);
        if (startLine) tempLineS.set(x, y);
    }
}
//--------------------------------------------------------------
void DrawBezPoly::mouseDragged(int x, int y, int button){
    if (drawingLine){
        if(button==2){
            if (!endingBezier){
                bez1.set(x, y);
                tempLineE.set(x, y);
            }else{
                bez2.set(x,y);
            }
        }else{
            drawing.addVertex(x, y); 
            bezStart.set(x,y);
            bez1.set(x,y);
            bez2.set(x,y);
            bezEnd.set(x,y);
            tempLineS.set(x, y);
            tempLineE.set(x, y);
            tagNum = XML2.addTag("PT");
            XML2.setAttribute("PT", "type:", "addVertex",tagNum);
            XML2.setValue("PT:X", x, tagNum);
            XML2.setValue("PT:Y", y, tagNum);
            tagNum++;
        }
    }
}
//--------------------------------------------------------------
void DrawBezPoly::mousePressed(int x, int y, int button){
    if (drawingLine){
        if(button==2){
            if (!endingBezier){
                if(startLine){
                    drawing.addVertex(x, y); 
                    tagNum = XML2.addTag("PT");
                    XML2.setAttribute("PT", "type:", "addVertex",tagNum);
                    XML2.setValue("PT:X", x, tagNum);
                    XML2.setValue("PT:Y", y, tagNum);
                    tagNum++;
                    bezStart.set(x,y);
                    bez1.set(x,y);
                    bez2.set(x,y);
                    bezEnd.set(x,y); 
                    tempLineS.set(x, y);
                    tempLineE.set(x, y);
                    startLine = false;
                }else{
                    bezStart.set(bezEnd); 
                    tempLineS.set(bezEnd);
                }
                tempCurve = false;
                bez1.set(x, y);
            }else{
                bez2.set(x,y);
                bezEnd.set(x,y);
                tempLineS.set(x, y);
                tempLineE.set(x, y);
                tempCurve = true;
            }
        }else{
            drawing.addVertex(x, y);
            bezStart.set(x,y);
            bez1.set(x,y);
            bez2.set(x,y);
            bezEnd.set(x,y);
            tempLineS.set(x, y);
            startLine = false;
            tagNum = XML2.addTag("PT");
            XML2.setAttribute("PT", "type:", "addVertex",tagNum);
            XML2.setValue("PT:X", x, tagNum);
            XML2.setValue("PT:Y", y, tagNum);
            tagNum++;
        }
    }
}
//--------------------------------------------------------------
void DrawBezPoly::mouseReleased(int x, int y, int button){
    if (drawingLine){
        if(button==2){
            if (!endingBezier){
                bez1.set(x, y);
                endingBezier = true;
            }else{
                bez2.set(x,y);
                drawing.bezierTo(bez1, bez2, bezEnd);
                tagNum = XML2.addTag("PT");
                XML2.setAttribute("PT", "type:", "bezierTo",tagNum);
                XML2.setValue("PT:CX1",bez1.x, tagNum);
                XML2.setValue("PT:CY1",bez1.y, tagNum);
                XML2.setValue("PT:CX2",bez2.x, tagNum);
                XML2.setValue("PT:CY2",bez2.y, tagNum);
                XML2.setValue("PT:X",bezEnd.x, tagNum);
                XML2.setValue("PT:Y",bezEnd.y, tagNum);
                tagNum++;
                tempCurve = false;
                endingBezier = false;
                bezStart.set(bezEnd);
                bez1.set(bezEnd);
            }
        }else{
            bez1.set(x,y);
            bez2.set(x,y);
            bezEnd.set(x,y);
        }
    }
}