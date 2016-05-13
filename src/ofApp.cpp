//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
//

#pragma once
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
#define MIN_MILLIS_BETWEEN_EXPRESSIONS 2000
#define RANDOM_MILLIS_ADDED_BETWEEN_EXPRESSIONS 2000
#define LOWER_EXPRESSION_THRESHOLD 0.4
#define UPPER_EXPRESSION_THRESHOLD 0.75

void ofApp::setup() {
    debug = false;

	cam.initGrabber(WIDTH, HEIGHT);
    scaleRatio = ofGetHeight()/HEIGHT;
	tracker.setup();
	tracker.setRescale(.5);
    classifier.load("expressions");

    flippedCam.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);

    ofRegisterURLNotification(this);

    faceColor = ofColor(0,0,0);
    faceLineWidth = 3;

    ofHideCursor();
    
    happyTimer.set(0);
    happyTimer.start();
    
    sadTimer.set(0);
    sadTimer.start();
    
    happyThreshold = UPPER_EXPRESSION_THRESHOLD;
    sadThreshold = UPPER_EXPRESSION_THRESHOLD;
    
    font_original = new ofxTrueTypeFontUC();
    font_original->load("OpenSansEmoji.ttf", 20, true, true);
}

void ofApp::update() {
    for (int i=0; i<tweets.size(); i++) {
        tweets[i].update();
    }

    for (int i=0; i<tweets.size(); i++) {
        if (tweets[i].dead) {
            tweets.erase(tweets.begin() + i);
            continue;
        }
    }

	cam.update();

    ofImage smallCam;

    if(cam.isFrameNew()) {
        flippedCam = cam.getPixels();
        flippedCam.mirror(false, true);

        if (VERTICAL) flippedCam.rotate90(3);

        if(tracker.update(toCv(flippedCam))) {
            classifier.classify(tracker);
        }
        
        faceColor = ofColor(0,0,0);

        float happy = classifier.getProbability(0);
        float neutral = classifier.getProbability(1);
        float sad = classifier.getProbability(2);
        
        faceColor.g = ofMap(happy, 0, 1, 0, 255);
        faceColor.r = ofMap(sad, 0, 1, 0, 255);
        
        if (classifier.getPrimaryExpression() == 1) {
            faceColor = ofColor(255,255,255);
        }

        int primaryExpression = classifier.getPrimaryExpression();
        float primaryExpressionProbability = classifier.getProbability(primaryExpression);
        faceLineWidth = ofMap(primaryExpressionProbability, 0, 1, 0, 8);
        
        float happyProbability = classifier.getProbability(0);
        float sadProbability = classifier.getProbability(2);
        
        if (happyTimer.finished()){
            if (happyProbability >= happyThreshold) {
                if (prevState == "sad") {
                    for (int i = 0; i < tweets.size(); i++) {
                        tweets[i].fade = true;
                    }
                }
                
                sendExpression();
                
                happyTimer.reset(MIN_MILLIS_BETWEEN_EXPRESSIONS + ofRandom(RANDOM_MILLIS_ADDED_BETWEEN_EXPRESSIONS));
                sadTimer.reset(0);
                
                happyThreshold = LOWER_EXPRESSION_THRESHOLD;
                sadThreshold = UPPER_EXPRESSION_THRESHOLD;
                
                prevState = "happy";
            }
            else if (happyProbability < happyThreshold) {
                happyTimer.reset(0);
                happyThreshold = UPPER_EXPRESSION_THRESHOLD;
            }
        }
        
        if (sadTimer.finished()){
            if (sadProbability >= sadThreshold) {
                if (prevState != "sad") {
                    for (int i = 0; i < tweets.size(); i++) {
                        cout << tweets.size() << " " << i << endl;
                        tweets[i].fade = true;
                    }
                }
                
                sendExpression();
                
                sadTimer.reset(MIN_MILLIS_BETWEEN_EXPRESSIONS + ofRandom(RANDOM_MILLIS_ADDED_BETWEEN_EXPRESSIONS));
                happyTimer.reset(0);
                
                sadThreshold = LOWER_EXPRESSION_THRESHOLD;
                happyThreshold = UPPER_EXPRESSION_THRESHOLD;
                
                prevState = "sad";
            }
            else if (sadProbability < sadThreshold) {
                sadTimer.reset(0);
                sadThreshold = UPPER_EXPRESSION_THRESHOLD;
            }
        }

        faceLocation = tracker.getImageFeature(ofxFaceTracker::NOSE_BRIDGE).getCentroid2D();
        faceLocation.x = faceLocation.x * scaleRatio;
        faceLocation.y = faceLocation.y * scaleRatio;
    }
}

void ofApp::urlResponse(ofHttpResponse & response) {
    if (response.status==200) {
        ofxJSONElement result = response.data.getText();
        searchError = false;

        Json::Value tweetsJSON = result["tweets"];
        for (Json::ArrayIndex i = 0; i < tweetsJSON.size(); i++) {
            ofPoint location = ofPoint(faceLocation.x, faceLocation.y - 350);
            Tweet tweet;
            tweet.setup(font_original, location, tweetsJSON[i]["text"].asString(), tweetsJSON[i]["username"].asString(), tweetsJSON[i]["sentiment"]["compound"].asFloat());
            tweets.push_back(tweet);
        }
    } else {
        cout << "Failed: " << response.status << " " << response.error << endl;
        searchError = true;
    }
}

void ofApp::sendExpression() {
    int primary = classifier.getPrimaryExpression();
    expression = classifier.getDescription(primary);

    string url = "";
    if (expression == "sad") {
        url = "http://localhost:5000/search?&neg_rel=$gt&neg_value=0.4&randomize_results=True&max_results=1";
    } else if (expression == "happy") {
        url = "http://localhost:5000/search?&pos_rel=$gt&pos_value=0.4&randomize_results=True&max_results=1";
    }

    if (url=="") return;

    ofLoadURLAsync(url);
}

void ofApp::drawDebuggingTools() {
    ofSetLineWidth(2);

    int w = 100, h = 12;
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(5, 10);
    int n = classifier.size();
    int primary = classifier.getPrimaryExpression();
    for(int i = 0; i < n; i++){
        ofSetColor(i == primary ? ofColor::red : ofColor::black);
        ofDrawRectangle(0, 0, w * classifier.getProbability(i) + .5, h);
        ofSetColor(255);
        ofDrawBitmapString(classifier.getDescription(i) + " " + to_string(classifier.getProbability(i)), 5, 9);
        ofTranslate(0, h + 5);
    }
    ofDrawBitmapString(faceColor.g, 5, 9);
    ofTranslate(0, h + 5);
    ofDrawBitmapString(faceColor.r, 5, 9);
    ofTranslate(0, h + 5);
    ofDrawBitmapString("Tweet count: " + to_string(tweets.size()), 5, 9);
    ofTranslate(0, h + 5);
    ofDrawBitmapString("Framerate: " + to_string(ofGetFrameRate()), 5, 9);
    ofTranslate(0, h + 5);
    ofDrawBitmapString("Happy threshold:  " + to_string(happyThreshold), 5, 9);
    ofTranslate(0, h + 5);
    ofDrawBitmapString("Sad threshold:  " + to_string(sadThreshold), 5, 9);
    ofTranslate(0, h + 5);
    if (searchError) {
        ofDrawBitmapString("Could not connect to server (at frame: " + to_string(ofGetFrameNum()) + ")", 5, 9);
    }
    ofPopMatrix();
    ofPopStyle();

    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
    ofShowCursor();
}


void ofApp::draw() {
    ofPushMatrix();
    //camera stuff
    ofScale(scaleRatio, scaleRatio);
    flippedCam.draw(0,0);
    //tracker stuff
    ofPushStyle();
    ofSetColor(faceColor);
    ofSetLineWidth(faceLineWidth);
    tracker.draw();
    ofPopStyle();
    ofPopMatrix();

    //ofCircle(faceLocation.x, faceLocation.y, 30);
    //tweet stuff
    for (int i=0; i<tweets.size(); i++) {
        tweets[i].draw();
    }
    if (debug) {
        drawDebuggingTools();
    }
}

void ofApp::keyPressed(int key) {
    if (key == 'd') {
        debug = !debug;
        ofHideCursor();
    }
    if (key == 'c') {
        Tweet tweet;
        ofPoint location = ofPoint(ofGetWidth() / 2, ofGetHeight() - 350);
        tweet.setup(font_original, location, "mini tweet", "aguy", 3);
        tweets.push_back(tweet);
    }
	if(key == 'f') {
		tracker.reset();
		classifier.reset();
	}
	if(key == 'e') {
		classifier.addExpression();
	}
	if(key == 'a') {
		classifier.addSample(tracker);
	}
	if(key == 's') {
		classifier.save("expressions");
	}
	if(key == 'l') {
		classifier.load("expressions");
	}

    if(key == 'h') {
        classifier.save("expressions");
    }
    if(key == 'j') {
        classifier.save("expressions");
    }
}
