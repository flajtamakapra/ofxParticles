#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


    // Surcharge des fcts ofDist en ajoutant ofPoints
    // (idéalement le faire avec ofVect2f et ofVect3f
    float ofDist(ofPoint p1, ofPoint p2){
         return sqrt(pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2));
    }
    float ofDist(ofPoint p1, float x2, float y2){
         return sqrt(pow((p1.x-x2),2)+pow((p1.y-y2),2));
    }

    vector<Particle> particles;
    int nbParticles = 1000;
    int mouseX = 0;
    int mouseY = 0;
    int pp1 = 0;
    int pp2 = 0;

    // ----
    // OpenCV
    ofVideoGrabber          vidGrabber;
    ofxCvColorImage			colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;

    ofxCvContourFinder      contourFinder;


    int                     threshold;
    bool                    bLearnBakground;
    int width = ofGetWidth();
    int height = ofGetHeight();
};
