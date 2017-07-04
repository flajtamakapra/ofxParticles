#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofSetVerticalSync(true);

    cam.setPosition(0, 0, 0);

    lookat.setPosition(1000,0, 0);
    cam.lookAt(lookat);


    for (int i = 0 ; i < nbParticles ; i++) {


        ofPoint pts;
        pts.set(ofGetWidth()/2, ofGetHeight()/2);
        float dim = 1;
        float radius = ofRandom(200,500);
        float sx, sy;
        sx = sy = ofRandom(-0.3,0.3);

        float sz = 1;
        float ox = ofRandom(100);
        float oy = ofRandom(100);
        ofColor color = ofRandom(100, 255);
        Particle p(pts, radius, dim, sx, sy, sz, ox, oy, color);
        particles.push_back(p);
    }

//    // ------
//    // OpenCV
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
                grayBg = grayImage;
                bLearnBakground = false;
            }
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(threshold);

            contourFinder.findContours(grayDiff, 200, 500, 20, false);	// find holes
        }
        if(contourFinder.nBlobs<0){
            for (int i = 0 ; i < contourFinder.blobs[0].nPts ; i+=10) {

                int nextPts = 0;
                for(int j = 0 ; j < nbParticles ; j++) {
                    float d1 = ofDistSquared(particles[j].pts, trackCv[i].pts);
                    float d2 = ofDistSquared(particles[nextPts].pts, trackCv[i].pts);
                    if(d1 < d2){
                        nextPts = j;
                    }
                }
                cout << nextPts << endl;
                trackCv.push_back(particles[nextPts]);

            }

        }

    }
    //--------------------------------------


    int p0 = 0; // point initial: curseur.
    for(int i = 0 ; i < nbParticles ; i++ ) {
        particles[i].update();
        float d1 = ofDistSquared(particles[i].pts, mouseX, mouseY);
        float d2 = ofDistSquared(particles[p0].pts, mouseX, mouseY);
        if(d1 < d2) {
            p0 = i;
        }
    }
    if(cursorTrack){
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
cam.begin();
    if(cvTrack){

        string nbb = std::to_string(contourFinder.nBlobs);
        string nbBlobs = "nbBlobs: "  + nbb;
        ofDrawBitmapString(nbBlobs, 100, 40,0 );
        for (int i = 0 ; i < contourFinder.nBlobs ; i++){
            string npts = std::to_string(contourFinder.blobs[i].nPts);
            ofDrawBitmapString("Blob " + std::to_string(i) + ": " + npts, 100, (i+3)*20, 0);
        }
    }


    for (int i = 0 ; i < nbParticles ; i++)particles[i].draw();
    if(cursorTrack){
        ofMesh m;
        for(int i = 0 ; i < track.size() ; i++){
            ofSetColor(ofMap(track[i].pts.x*track[i].pts.y, 0, width*height, 0, 255));
            m.addVertex(track[i].pts);
            m.addIndex(i);
         }

//   m.drawWireframe();
     m.draw();
     ofSetColor(255);

//    for(int i = 1 ; i < track.size()-1 ; i++) {
//        ofDrawLine(track[i].pts, track[i+1].pts);
//    }
         track.clear();
    }

    if(cvTrack && trackCv.size() > 0){
        for(int i = 1 ; i < trackCv.size()-1 ; i++) {
            ofDrawLine(trackCv[i].pts, trackCv[i+1].pts);
        }
    trackCv.clear();
    }

    cam.end();

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
    case 's':
        if(constellation ==true){
            constellation = false;
        }
        else constellation = true;
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
