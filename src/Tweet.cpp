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

#define BIRD_SIZE 50
#define BIRD_PADDING 20

Tweet::TweetColors Tweet::getTweetColor(int alpha) {
    Tweet::TweetColors colors;
    colors.bgColor = ofColor(255,255,255,alpha);
    colors.textColor = ofColor(0);
    colors.birdColor = ofColor(255,255,255);

    if (moodLevel < 0) {
        colors.bgColor.set(255,255,255,alpha);
        colors.textColor.set(0,0,0,alpha);
        colors.birdColor.set(255,0,0,alpha);
    } else if (moodLevel > 0) {
        colors.bgColor.set(255,255,255,alpha);
        colors.birdColor.set(0,255,0,alpha);
    }

    return colors;
}

Tweet::Tweet(){

}

void Tweet::setup(ofPoint _location, string tweetContent, string tweetAuthor, float _moodLevel, int paddingWidth, int _paddingHeight) {
    ofTrueTypeFont::setGlobalDpi(72);
    font.load("Helvetica.ttf", 24, true, true);//, true);
    location = _location;
//    location.y -= 100;
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
    initLocationY = location.y;
    initLocationX = location.x;
    endLocationY = 0;
    endLocationX = ofRandom(0, 1) > 0.5 ? stringBox.width / 2 : ofGetWidth() - stringBox.width / 2;

    bird.load("twitter-bird.png");

    //cloud stuff
    totalRays = 20;
    stepSize = 100;
    ellipseWidthRad = (stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2;
    ellipseHeightRad = stringBox.height / 2;
    noiseSeed = ofRandom(10000);

    angleStep = 360.0 / totalRays;
    ofSetCircleResolution(60);

    for (int i=0; i<totalRays; i++)
    {
        float x = 0 + ellipseWidthRad * cos(ofDegToRad(angleStep*i));
        float y = 0 + ellipseHeightRad * sin(ofDegToRad(angleStep*i));
        bubblePoints.push_back(ofPoint(x,y));
    }

}

void Tweet::update() {
    if (alpha < 255) {
        alpha+=5;
        if (alpha > 255) {
            alpha = 255;
        }
    }

    colors = getTweetColor(alpha);

    if (location.y < 0) {
        display = false;
    }

    auto durationY = 10.f;
    auto durationX = 3.f;
    auto endTimeY = initTime + durationY;
    auto endTimeX = initTime + durationX;
    auto now = ofGetElapsedTimef();

    location.y = ofxeasing::map(now, initTime, endTimeY, initLocationY, endLocationY, &ofxeasing::linear::easeInOut);



    if (now < endTimeX) {
      cout << "end x: " << endLocationX << endl;
      cout << "actual x: " << location.x << endl;
      location.x = ofxeasing::map(now, initTime, endTimeX, initLocationX, endLocationX, &ofxeasing::elastic::easeOut);
    } else {
      // location.x
    }


    noiseSeed+=0.02;
}

void Tweet::draw() {

    if (bubblePoints.size()>0) {
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(location);
    ofSetColor(colors.bgColor);

    //BUBBLE SHAPE
    ofBeginShape();
        ofNoFill();
    // start controlpoint
    float scale = ofMap(ofNoise(noiseSeed + (totalRays-1)*5.3),0,1,0.8, 1.5);
    ofCurveVertex(bubblePoints[totalRays-1]);
    // only these points are drawn //////
    for (int i=0; i<totalRays; i++)
    {
        scale = ofMap(ofNoise(noiseSeed + i*5.3),0,1,0.8, 1.5);
        ofCurveVertex(bubblePoints[i].x * scale,bubblePoints[i].y * scale);
        //ofDrawCircle(bubblePoints[i],5);
    }
    scale = ofMap(ofNoise(noiseSeed + 0*5.3),0,1,0.8, 1.5);
    ofCurveVertex(bubblePoints[0].x * scale,bubblePoints[0].y * scale);
    // end controlpoint
    scale = ofMap(ofNoise(noiseSeed + 1*5.3),0,1,0.8, 1.5);
    ofCurveVertex(bubblePoints[1].x * scale, bubblePoints[1].y * scale);
    ofEndShape();
    
    // STRING STUFF
   if (!wrappedString.empty()) {
       ofSetColor(colors.textColor);
       font.drawString(wrappedString, -(stringBox.width / 2) + ((BIRD_SIZE + BIRD_PADDING) / 2), font.getLineHeight() - (stringBox.height / 2));
   }
    ofPopStyle();

   // BIRD STUFF
   ofPushStyle();
   ofSetColor(colors.birdColor);
   ofFill();
   bird.draw( ofPoint(-((stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2), -stringBox.height / 2 + BIRD_SIZE / 2) , 50, 40.65);
    ofPopStyle();
    ofPopMatrix();
  }
}
