//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
//

#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCvHaarFinder.h"
#include "ofxJSON.h"
#include "Tweet.hpp"
#include "Timer.hpp"
#include "ofxTrueTypeFontUC.h"

#define RECEIVER_PORT 12345
#define SENDER_PORT 12346
#define SENDER_HOST "localhost"
#define TWEET_LIFESPAN 300
#define CAM_SCALE 20
#define WIDTH 640.0
#define HEIGHT 480.0
#define VERTICAL true
#define USE_SERVER true

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void sendExpression();
    void drawDebuggingTools();
    void urlResponse(ofHttpResponse & response);

    ofVideoGrabber cam;
    ofImage flippedCam;
    ofxFaceTracker tracker;
    ofPoint faceLocation;
    vector<Tweet> tweets;
    bool debug;
    bool searchError;
    float scaleRatio;
	ExpressionClassifier classifier;
    ofColor faceColor;
    int faceLineWidth;
    string expression;
    Timer happyTimer;
    Timer sadTimer;
    
    string prevState;
    float happyThreshold;
    float sadThreshold;

private:
    ofxTrueTypeFontUC* font_original;
};
