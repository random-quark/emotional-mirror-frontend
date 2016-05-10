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
#include <algorithm>

#define BIRD_SIZE 50
#define BIRD_PADDING 20
#define MIN_RADIUS 70
#define BUBBLE_PADDING 10

Tweet::Tweet(){

}

void Tweet::setup(ofxTrueTypeFontUC* _font, ofPoint _location, string tweetContent, string tweetAuthor, float _moodLevel, int paddingWidth, int _paddingHeight) {
    location = _location;

    moodLevel = _moodLevel;
    display = true;
    paddingHeight = _paddingHeight;

    text = "@" + tweetAuthor + " " + tweetContent;
    
    font = _font;
    wrappedString = Util::wrapString(text, 350, font);
    stringBox = font->getStringBoundingBox(wrappedString,0,0);

    int width = stringBox.width + paddingWidth;
    int height = stringBox.height + paddingHeight;

    alpha = 0;
    colors = getTweetColor();

    //easing
    initTime = ofGetElapsedTimef();
    initLocationY = location.y;
    initLocationX = location.x;
    endLocationY = 0;
    minX = (stringBox.width / 2) + BIRD_SIZE + BIRD_PADDING;
    maxX = (ofGetWidth() - stringBox.width / 2) - (BIRD_SIZE + BIRD_PADDING);
    endLocationX = ofRandom(0, 1) > 0.5 ? minX : maxX;
    movementNoiseSeed = ofRandom(0, 1000);

    bird.load("twitter-bird.png");

    //cloud stuff
    cornerRadius = 60;
    noiseStep = 40;
    resamplingRate = 40;
    
    ellipseWidthRad = (stringBox.width + BIRD_SIZE + BIRD_PADDING);
    ellipseHeightRad = stringBox.height;
    
    ellipseHeightRad = max(ellipseHeightRad, MIN_RADIUS);
    path.rectRounded(-ellipseWidthRad/2, (-ellipseHeightRad/2)-BUBBLE_PADDING, ellipseWidthRad, ellipseHeightRad, cornerRadius);
    
    ofPolyline polyline = path.getOutline()[0];
    polyline = polyline.getResampledBySpacing(resamplingRate);
    originEllipsePoints = polyline.getVertices();
    
    for (int i = 0; i<originEllipsePoints.size(); i++){
        if (i==originEllipsePoints.size()-1 && ofDist(originEllipsePoints[0].x, originEllipsePoints[0].y, originEllipsePoints[originEllipsePoints.size()-1].x, originEllipsePoints[originEllipsePoints.size()-1].y)<20)
        {
            // if the last point in resampling is too close to the first then
            // do nothing (ie. don't add it to the list of bubble points to draw)
        }
        else
        {
            noiseSeeds.push_back(ofRandom(10000));
            normVectors.push_back(originEllipsePoints[i].getNormalized());
            newVec = originEllipsePoints[i] + normVectors[i] * ofNoise(noiseSeeds[i]) * noiseStep;
            ellipsePoints.push_back(newVec);
        }
    }
}

void Tweet::update() {
    colors = getTweetColor();
    
    if ((location.y + ellipseHeightRad * 2) < 0) {
        display = false;
    }

    auto durationY = 10.f;
    auto durationX = 3.f;
    auto endTimeY = initTime + durationY;
    auto endTimeX = initTime + durationX;
    auto now = ofGetElapsedTimef();

    location.y = ofxeasing::map(now, initTime, endTimeY, initLocationY, endLocationY, &ofxeasing::linear::easeInOut);

    if (now < endTimeX) {
      location.x = ofxeasing::map(now, initTime, endTimeX, initLocationX, endLocationX, &ofxeasing::quint::easeOut);
    } else {
      location.x += ofMap(ofNoise(movementNoiseSeed), 0, 1, -8, 8);
      movementNoiseSeed += 0.03;
    }

    // adding noise to the bubble
    for (int i = 0; i<ellipsePoints.size(); i++){
        noiseSeeds[i]+=0.02;
        newVec = originEllipsePoints[i] + normVectors[i] * ofNoise(noiseSeeds[i]) * noiseStep;
        ellipsePoints[i] = newVec;
    }

}

void Tweet::draw() {
    if (ellipsePoints.size()>0) {
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(location);
    
    // BUBBLE STUFF
    ofSetColor(colors.shadowColor);
    drawBubble();
    ofSetColor(colors.bubbleColor);
    ofTranslate(-7,-7);
    drawBubble();
        
    // STRING STUFF
    if (!wrappedString.empty()) {
       ofSetColor(colors.textColor);
       font->drawString(wrappedString, -(stringBox.width / 2) + ((BIRD_SIZE + BIRD_PADDING) / 2), - ((stringBox.height - font->getLineHeight()) / 2));
   }
    ofPopStyle();

   // BIRD STUFF
   ofPushStyle();
   ofSetColor(colors.birdColor);
   ofFill();
   bird.draw( ofPoint(-((stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2), -stringBox.height / 2) , 50, 40.65);
    ofPopStyle();
    ofPopMatrix();
  }
}

void Tweet::drawBubble(){
    ofBeginShape();
    // start controlpoint
    ofCurveVertex(ellipsePoints[ellipsePoints.size()-1]);
    // only these points are drawn
    for (int i=0; i<ellipsePoints.size(); i++){
        ofCurveVertex(ellipsePoints[i]);
    }
    ofCurveVertex(ellipsePoints[0]);
    // end controlpoint
    ofCurveVertex(ellipsePoints[1]);
    ofEndShape();
}

Tweet::TweetColors Tweet::getTweetColor() {
    Tweet::TweetColors colors;
    colors.bubbleColor = ofColor(255,255,255,0);
    colors.textColor = ofColor(0,0,0,alpha);
    colors.birdColor = ofColor(255,255,255);
    colors.shadowColor = ofColor(50, alpha*0.75);
    
    if (moodLevel < 0) {
        colors.bubbleColor.set(255,255,255,alpha);
        colors.textColor.set(0,0,0,alpha);
        colors.birdColor.set(255,0,0,alpha);
        colors.shadowColor.set(50, alpha*0.75);
    } else if (moodLevel > 0) {
        colors.bubbleColor.set(255,255,255,alpha);
        colors.birdColor.set(0,255,0,alpha);
        colors.shadowColor.set(50, alpha*0.75);
    }

    alpha+=5;
    alpha=ofClamp(alpha,0,255);

    return colors;
}
