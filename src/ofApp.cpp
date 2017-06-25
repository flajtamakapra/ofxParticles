#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    for (int i = 0 ; i < nbParticles ; i++) {


        ofPoint pts;
        pts.set(ofRandom(-10,ofGetWidth()),ofRandom(0, ofGetHeight()),0);
        int dim = ofRandom(2.5);
        float sx = ofRandom(-0.5,0.7);
        float sy = ofRandom(-0.1,0.5);
        float sz = ofRandom(1);
        ofColor color = ofRandom(100, 255);
        Particle p(pts, dim, sx, sy, sz, color);
        particles.push_back(p);
    }

    // ------
    // OpenCV
//    vidGrabber.setVerbose(true);
//    vidGrabber.setup(width, height);
//    vidGrabber.listDevices();

//    colorImg.allocate(width, height);
//    grayImage.allocate(width, height);
//    grayBg.allocate(width, height);
//    grayDiff.allocate(width, height);

//    bLearnBakground = true;
//    threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){

    //--------------------------------------
    // -------------------------------------
    // OpenCV
    if(cvTrack){
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
            contourFinder.findContours(grayDiff, 200, 500, 20, false);	// find holes
        }

    }

    // 1 trouver le point le plus près de mouseX / mouseY
    //  Comparer particles[i].pts avec le curseur, si ce point est plus près que p0, p0 = i et push_back p0;
    // 2 faire 100 fois:
    //   trouver le point le plus près de ce point, qui n'est pas dans le vecteur.

    if(cursorTrack){

        int p0 = 0; // point initial: curseur.
        for(int i = 0 ; i < nbParticles ; i++ ) {
            particles[i].update();
            float d1 = ofDistSquared(particles[i].pts, mouseX, mouseY);
            float d2 = ofDistSquared(particles[p0].pts, mouseX, mouseY);
            if(d1 < d2) {
                p0 = i;
            }
        }
        track.push_back(particles[p0]);

        for (int i = 0 ; i < 60; i++ ){
            int nextPts = 0;


            for (int j = 0 ; j < nbParticles ; j++) {
                float d1 = ofDistSquared(particles[j].pts, track[i].pts);
                float d2 = ofDistSquared(particles[nextPts].pts, track[i].pts);


                if(d1 < d2 && !isVisited(particles[j])){
                    nextPts = j;
                }

            }

            track.push_back(particles[nextPts]);
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    if(cvTrack){

        string nbb = std::to_string(contourFinder.nBlobs);
        string nbBlobs = "nbBlobs: "  + nbb;
        ofDrawBitmapString(nbBlobs, 100, 40,0 );
        for (int i = 0 ; i < contourFinder.nBlobs ; i++){
            string npts = std::to_string(contourFinder.blobs[i].nPts);
            ofDrawBitmapString("Blob " + std::to_string(i) + ": " + npts, 100, (i+3)*20, 0);
        }
    }


    for (int i = 0 ; i < nbParticles ; i++) {

        particles[i].draw();
    }
    if(cursorTrack){

        ofMesh m;
         for(int i = 0 ; i < track.size() ; i++){
             ofSetColor(ofMap(mouseX*mouseY, 0, width*height, 0, 255));
             m.addVertex(track[i].pts);
             m.addIndex(i);
         }

     //    m.drawWireframe();
         m.draw();
         ofSetColor(255);

     //    for(int i = 1 ; i < track.size()-1 ; i++) {
     //        ofDrawLine(track[i].pts, track[i+1].pts);
     //    }
         track.clear();
    }

    if(cvTrack){
        if(contourFinder.nBlobs > 0){
            ofSetColor(255);
            ofSetPolyMode(OF_POLY_WINDING_ODD);
            ofSetColor(100,100,10,200);
            ofBeginShape();
            for (int i = 0 ; i < contourFinder.blobs[0].nPts ; i+=10) {
                int a = 0;
                for (int j = 0 ; j < nbParticles ; j++) {
                    float t1 = ofDistSquared(contourFinder.blobs[0].pts[i], particles[j].pts);
                    float t2 = ofDistSquared(contourFinder.blobs[0].pts[i], particles[a].pts);
                    if(t1 < t2){
                        a = i;
                        ofVertex(particles[a].pts.x, particles[a].pts.y);
                    }
                }
            }
            ofEndShape(true);

        }

    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
        case 'c':
            if(cursorTrack == true){
                cursorTrack = false;
            }
            else cursorTrack = true;
        break;
        case 'v':
            if(cvTrack == true){
                cvTrack = false;
            }
            else cvTrack = true;

            break;
    }
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
