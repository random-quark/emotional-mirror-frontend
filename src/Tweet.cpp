//
//  Tweet.cpp
//  emotion-mirror-4
//
//  Created by Tom Chambers on 18/03/2016.
//
//

#include "Tweet.hpp"
#include "ofApp.h"
#include "Util.hpp"
#include "ofxEasing.h"

Tweet::TweetColors Tweet::getTweetColor(int alpha) {
    Tweet::TweetColors colors;
    colors.bgColor = ofColor(255,255,255,alpha);
    colors.textColor = ofColor(0);
    colors.birdColor = ofColor(255,255,255);
    
    if (moodLevel == 3) {
        colors.bgColor.set(255,255,255,alpha);
        colors.textColor.set(0,0,0,alpha);
        colors.birdColor.set(255,0,0,alpha);
    } else if (moodLevel == 1) {
        colors.bgColor.set(255,255,255,alpha);
        colors.birdColor.set(0,255,0,alpha);
    }

    return colors;
}

Tweet::Tweet(){

}

void Tweet::setup(ofPoint _location, string tweetContent, string tweetAuthor, int _moodLevel, int paddingWidth, int _paddingHeight) {
    ofTrueTypeFont::setGlobalDpi(72);
    font.load("Helvetica.ttf", 24, true, true);//, true);
    location = _location;
    //profileImage.load(profileImageUrl);
    moodLevel = _moodLevel;
    display = true;
    paddingHeight = _paddingHeight;
    
    text = "@" + tweetAuthor + " " + tweetContent;
    
    wrappedString = Util::wrapString(text, 350, font);
    stringBox = font.getStringBoundingBox(wrappedString,0,0);
    
    int width = stringBox.width + paddingWidth;
    int height = stringBox.height + paddingHeight;
    
    alpha = 0;
    
    //easing
    initTime = ofGetElapsedTimef();
    initLocation = location.y;
    //cout << "starting at " << initLocation;
    endLocation = 0;
    
    bird.load("twitter-bird.png");
    
    //cloud stuff
    totalRays = 20;
    stepSize = 200;
    ellipseWidthRad = 200;
    ellipseHeightRad = 100;
    noiseSeed = ofRandom(10000);
    
    angleStep = 360.0 / totalRays;
    ofSetCircleResolution(60);
    
    for (int i=0; i<totalRays; i++)
    {
//        noiseSeeds.push_back(ofRandom(10000));

        float tempRad = sqrt(pow(ellipseHeightRad*cos(ofDegToRad(i*angleStep)),2) + pow(ellipseWidthRad*sin(ofDegToRad(i*angleStep)),2));
        radii.push_back(tempRad);
    }

}

void Tweet::update() {

    if (alpha < 255) {
        alpha+=20;
        if (alpha > 255) {
            alpha = 255;
        }
    }

    colors = getTweetColor(alpha);
    
    if (location.y < 0) {
        //cout << "location y gone " << location.y << endl;
        display = false;
    }
    
    auto duration = 4.f;
    auto endTime = initTime + duration;
    auto now = ofGetElapsedTimef();
    location.y = ofxeasing::map(now, initTime, endTime, initLocation, endLocation, &ofxeasing::quad::easeIn);
    
    //cout << "new loc " << location.y << endl;
    
//    //bubble stuff
    bubblePoints.clear();
    for (int i=0; i<totalRays; i++)
    {
        float noisedRadius = radii[i] + ofNoise(noiseSeed + i*5.3) * stepSize;
        float endX = sin(ofDegToRad(i*angleStep)) * noisedRadius;
        float endY = cos(ofDegToRad(i*angleStep)) * noisedRadius;

        bubblePoints.push_back(ofPoint(endX, endY));
    }
    noiseSeed+=0.01;
}

void Tweet::draw() {
    ofPushStyle();
    ofTranslate(location);
    
    ofSetColor(253, 202, 19);
    
    ofBeginShape();
    // start controlpoint
    ofCurveVertex(bubblePoints[totalRays-1]);
    
    // only these points are drawn
    for (int i=0; i<totalRays; i++)
    {
        ofCurveVertex(bubblePoints[i]);
    }
    ofCurveVertex(bubblePoints[0]);
    
    // end controlpoint
    ofCurveVertex(bubblePoints[1]);
    ofEndShape();

    
    if (!wrappedString.empty()) {
        
        ofSetColor(colors.textColor);
        
        font.drawString(wrappedString, 0, font.getLineHeight() + (paddingHeight / 4));
    }
    ofPopStyle();

    ofPushStyle();
    ofSetColor(colors.birdColor);
    ofFill();
    bird.draw(ofPoint(0,0), 50, 50);
    ofPopStyle();
}
