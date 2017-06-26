#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxOpenCv.h"
#include "math.h"

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

    float ofDistSquared(ofPoint p1, float x2, float y2){return pow((p1.x-x2),2)+pow((p1.y-y2),2);}
    float ofDistSquared(ofPoint p1, ofPoint p2){return pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2);}

    bool isVisited(Particle p){
        bool visite = false;
        for (int i = 0 ; i < track.size() ; i++){
            if(visite = (track[i] == p)){
                return true;
            }
        }
        return visite;
    };

    vector<Particle> particles;
    vector<Particle> track;
    vector<Particle> trackCv;
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
    int                     width = ofGetWidth();
    int                     height = ofGetHeight();

    bool                    cursorTrack = false;
    bool                    cvTrack = false;
    bool                    constellation = true;

};
