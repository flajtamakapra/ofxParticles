#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    for (int i = 0 ; i < nbParticles ; i++) {


        ofPoint pts;
        pts.set(ofRandom(-10,ofGetWidth()),ofRandom(0, ofGetHeight()),0);
        int dim = ofRandom(2.5);
        float sx = ofRandom(2,10);
        float sy = ofRandom(-3, 3);
        float sz = ofRandom(-5,5);
        ofColor color = ofRandom(100, 255);
        Particle p(pts, dim, sx, sy, sz, color);
        particles.push_back(p);
    }

    // ------
    // OpenCV
    vidGrabber.setVerbose(true);
    vidGrabber.setup(width, height);
    vidGrabber.listDevices();

    colorImg.allocate(width, height);
    grayImage.allocate(width, height);
    grayBg.allocate(width, height);
    grayDiff.allocate(width, height);

    bLearnBakground = true;
    threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){

    //--------------------------------------
    // -------------------------------------
    // OpenCV
    bool bNewFrame = false;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    if (bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg;
        if (bLearnBakground == true){
            grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }

        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 2000, (width*height)/3, 5, false);	// find holes
    }

    for (int i = 0; i < contourFinder.blobs.size() ; i++){

            for (int j = 0 ; j < contourFinder.blobs[i].nPts ; j++) {

//                int blobX = ofMap(contourFinder.blobs[i].pts[j].x, 0, width, 0, height);
//                int blobY = ofMap(contourFinder.blobs[i].pts[j].y, 0, height, 0, height);
            }

        }


    int p1, p2;
    p1 = 0;

    for (int i = 0 ; i < nbParticles ; i++){

        particles[i].update();
        if(ofDist(particles[i].pts, mouseX, mouseY) < ofDist(particles[p1].pts, mouseX, mouseY)){
            if(p1 < nbParticles) p2 = p1;
            p1 = i;
        }
    }
    if(p1 < nbParticles && p2 < nbParticles){

        pp1 = p1;
        pp2 = p2;

    }

}

//--------------------------------------------------------------
void ofApp::draw(){




    for (int i = 0 ; i < nbParticles ; i++) {
        particles[i].draw();
    }
    ofSetColor(255);
    ofDrawLine(particles[pp1].pts.x, particles[pp1].pts.y,particles[pp2].pts.x, particles[pp2].pts.y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
