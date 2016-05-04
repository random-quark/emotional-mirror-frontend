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
#define VERTICAL false

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void ageTweets();
    void drawTweet(Tweet tweet, int i);
    void sendExpression();
    void drawDebuggingTools();
    void drawMood();
    void drawBars();
    void drawStandard();
    void drawThought(Tweet tweet);
    string expression;

    ofRectangle faceLocation;

    bool debug;
    bool searchError;

	ofVideoGrabber cam;
    ofImage flippedCam;
	ofxFaceTracker tracker;

    vector<Tweet> tweets;


	ExpressionClassifier classifier;

    ofColor faceColor;
    int faceLineWidth;

    void urlResponse(ofHttpResponse & response);

};
