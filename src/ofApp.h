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
#include "ofxXmlSettings.h"

#define RECEIVER_PORT 12345
#define SENDER_PORT 12346
#define SENDER_HOST "localhost"
#define TWEET_LIFESPAN 300
#define CAM_SCALE 20
#define CAMERA_WIDTH 640.0
#define CAMERA_HEIGHT 480.0
#define VERTICAL true
#define USE_SERVER true
#define MIN_MILLIS_BETWEEN_EXPRESSIONS 2000
#define RANDOM_MILLIS_ADDED_BETWEEN_EXPRESSIONS 2000
#define LOWER_EXPRESSION_THRESHOLD 0.5
#define UPPER_EXPRESSION_THRESHOLD 0.7
#define HAPPY 0
//#define NEUTRAL 1
#define SAD 1

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void sendExpression();
    void drawDebuggingTools();
    void urlResponse(ofHttpResponse & response);

    ofxXmlSettings XML;
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
    string absolutePath;

private:
    ofxTrueTypeFontUC* font_original, *font_smaller;
};
