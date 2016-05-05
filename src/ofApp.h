#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCvHaarFinder.h"
#include "ofxJSON.h"
#include "Tweet.hpp"

#define RECEIVER_PORT 12345
#define SENDER_PORT 12346
#define SENDER_HOST "localhost"
#define TWEET_LIFESPAN 300
#define CAM_SCALE 20
#define WIDTH 640.0
#define HEIGHT 480.0
#define VERTICAL true

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
};
