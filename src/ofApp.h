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
#define CAM_SCALE 4
#define WIDTH 1440
#define HEIGHT 900
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

    ofVec2f mouth;
    ofPolyline mouthLine;
    ofPolyline innerMouthLine;

    ofRectangle faceLocation;
    ofPoint prevLocation;

    ofxCvHaarFinder finder;

    ofImage tweetBackground;

    bool debug;
    bool searchError;

	ofVideoGrabber cam;
    ofImage flippedCam;
	ofxFaceTracker tracker;

    vector<Tweet> tweets;

    //ofTrueTypeFont helvetica;
    //ofTrueTypeFont futura;

	ExpressionClassifier classifier;

    ofColor faceColor;
    int faceLineWidth;

    void urlResponse(ofHttpResponse & response);

    int searchFailCount;
    bool searchInProgress;
};
