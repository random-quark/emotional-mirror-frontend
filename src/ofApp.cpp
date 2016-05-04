#pragma once
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofSetFullscreen(true);
    ofSetCircleResolution(500);
    debug = false;

	cam.initGrabber(WIDTH, HEIGHT);

	tracker.setup();
	tracker.setRescale(.5);
    classifier.load("expressions");

    flippedCam.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);

    finder.setup("haarcascade_frontalface_default.xml");

    ofRegisterURLNotification(this);
    searchFailCount = 1;

    faceColor = ofColor(0,0,0);
    faceLineWidth = 3;
}

void ofApp::update() {
    for (int i=0; i<tweets.size(); i++) {
        tweets[i].update();
    }

//    for (int i=0; i<tweets.size(); i++) {
//        if (!tweets[i].display) {
//            tweets.erase(tweets.begin() + i);
//            continue;
//        }
//    }


	cam.update();

    if(cam.isFrameNew()) {
        flippedCam = cam.getPixels();
        flippedCam.mirror(false, true);
    }

    ofImage smallCam;
    smallCam.clone(flippedCam);
    smallCam.resize(WIDTH / CAM_SCALE, HEIGHT / CAM_SCALE);
    finder.findHaarObjects(smallCam);

    int biggest = 0;
    int record = -1;
    for(unsigned int i = 0; i < finder.blobs.size(); i++) {
        if (finder.blobs[i].area > biggest) {
            record = i;
            biggest = finder.blobs[i].area;
        };
    }
    if (record < 0) {
        return;
    }
    faceLocation = finder.blobs[record].boundingRect;

    ofImage face;

    face.clone(flippedCam);

    int x = (faceLocation.x * CAM_SCALE) - 50;
    if (x < 0) x = 0;
    int y = (faceLocation.y * CAM_SCALE) - 50;
    if (y < 0) y = 0;
    int width = (faceLocation.width * CAM_SCALE) + 100;
    if (width + x > WIDTH) width = WIDTH - x;
    int height = (faceLocation.height * CAM_SCALE) + 100;
    if (height + y > HEIGHT) width = HEIGHT - y;
    face.crop(x, y, width, height);
    if(tracker.update(toCv(face))) {
        classifier.classify(tracker);
    }

    float happy = classifier.getProbability(0);
    float sad = classifier.getProbability(1);

    faceColor.g = ofMap(happy, 0, 1, 0, 255);
    faceColor.r = ofMap(sad, 0, 1, 0, 255);

    int primaryExpression = classifier.getPrimaryExpression();
    float primaryExpressionProbability = classifier.getProbability(primaryExpression);
    faceLineWidth = ofMap(primaryExpressionProbability, 0, 1, 0, 8);

    if (ofGetFrameNum() % 60) {
//        sendExpression();
    }
}

void ofApp::urlResponse(ofHttpResponse & response) {
    if (response.status==200) {
        ofxJSONElement result = response.data.getText();
        searchError = false;

        Json::Value tweetsJSON = result["tweets"];
        cout << "size tweet" << tweetsJSON.size() << endl;
        for (Json::ArrayIndex i = 0; i < tweetsJSON.size(); i++) {
            ofPoint location = ofPoint((faceLocation.x + faceLocation.width) * CAM_SCALE, faceLocation.y * CAM_SCALE);
            Tweet tweet;
            tweet.setup(location, tweetsJSON[i]["text"].asString(), tweetsJSON[i]["username"].asString(), tweetsJSON[i]["sentiment"]["compound"].asInt(), 0, 10);
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
    if (expression == "smile") {
        url = "http://localhost:5000/search?&comp_rel=$gt&comp_value=0.6&randomize_results=True&max_results=1";
    } else if (expression == "sad") {
        url = "http://localhost:5000/search?&comp_rel=$lt&comp_value=-0.6&randomize_results=True&max_results=1";
    }

    if (url=="") return;

    ofLoadURLAsync(url);
}

void ofApp::drawDebuggingTools() {
    ofSetLineWidth(2);

    ofPushMatrix();
    ofTranslate(faceLocation.x * CAM_SCALE - 50, faceLocation.y * CAM_SCALE - 50);
    tracker.draw();
    ofPopMatrix();

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
    ofPopMatrix();
    ofPopStyle();

    ofNoFill();
    for(unsigned int i = 0; i < finder.blobs.size(); i++) {
        ofRectangle cur = finder.blobs[i].boundingRect;
        ofDrawRectangle(cur.x * CAM_SCALE, cur.y * CAM_SCALE, cur.width * CAM_SCALE, cur.height * CAM_SCALE);
    }

    if (searchError) {
        ofDrawRectangle(0, 0, WIDTH, 20);
        ofDrawBitmapString("Could not connect to server", 200, 10);
    }

    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
}


void ofApp::draw() {
    ofPushMatrix();

    if (VERTICAL) {
        ofTranslate(WIDTH / 2, HEIGHT / 2);
        ofRotate(90);
        ofTranslate(-(WIDTH / 2), -(HEIGHT / 2));
    }

    flippedCam.draw(0,0);

    ofPushMatrix();
    ofPushStyle();
    ofSetColor(faceColor);
    ofSetLineWidth(faceLineWidth);
    ofTranslate(faceLocation.x * CAM_SCALE - 50, faceLocation.y * CAM_SCALE - 50);
    tracker.draw();
    ofPopStyle();
    ofPopMatrix();


    for (int i=0; i<tweets.size(); i++) {
        tweets[i].draw();
    }

    if (debug) {
        drawDebuggingTools();
    }
    ofPopMatrix();
}

void ofApp::keyPressed(int key) {
    if (key == 'd') {
        debug = !debug;
    }
    if (key == 'c') {
        Tweet tweet;
        cout << "boom" << endl;
        ofPoint location = ofPoint((faceLocation.x + (faceLocation.width / 2)) * CAM_SCALE, faceLocation.y * CAM_SCALE);
        tweet.setup(location, "My dog has died. I am very sad and upset. My dog has died. I am very sad and upset.  My dog has died. I am very sad and upset. ", "tom_d_chambers", 3, 0, 30);
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
