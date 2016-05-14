//  Tweet.cpp
//  emotion-mirror-4
//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
//

#include "Tweet.hpp"
#include "ofApp.h"
#include "Util.hpp"
#include "ofxEasing.h"
#include <algorithm>

#define BIRD_SIZE 35
#define BIRD_PADDING 25
#define MIN_HEIGHT 140
#define FBO_SAFETY_MARGIN 150
#define SHADOW_OFFSET 15
#define BUBBLE_TOP_LEFT_SHIFT 15  // bigger values push the text down to center it properly

Tweet::Tweet(){
}

void Tweet::setup(ofxTrueTypeFontUC* _font, ofPoint _location, string tweetContent, string tweetAuthor, string profileImageUrl, float _moodLevel) {
    location = _location;

    moodLevel = _moodLevel;
    fade = false;
    dead = false;

    author = "@" + tweetAuthor;
    text = author + " " + tweetContent;

    font = _font;
    wrappedString = Util::wrapString(text, 350, font);
    stringBox = font->getStringBoundingBox(wrappedString,0,0);

    alpha = 0;
    colors = getTweetColor();

    //easing
    initTime = ofGetElapsedTimef();
    initLocationY = location.y;
    initLocationX = location.x;
    endLocationY = 0;
    minX = ((stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2) + 100;                      // MIN location of tweet on x-axis
    maxX = ofGetWidth() - (stringBox.width - BIRD_SIZE + BIRD_PADDING) / 2 - 100;   // MAX location of tweet on x-axis
    endLocationX = ofRandom(0, 1) > 0.5 ? minX : maxX;
    movementNoiseSeed = ofRandom(0, 1000);

    bird.load("twitter-bird-48px.png");
    profileImage.load(profileImageUrl);

    //cloud stuff
    cornerRadius = 60;
    noiseStep = 40;
    resamplingRate = 40;

    bubbleWidth = (stringBox.width + BIRD_SIZE + BIRD_PADDING) + 40;
    bubbleHeight = stringBox.height + 40;

    bubbleHeight = max(bubbleHeight, MIN_HEIGHT);
    path.rectRounded(-bubbleWidth/2, -bubbleHeight/2, bubbleWidth, bubbleHeight, cornerRadius);

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

    //FBO Blur stuff
    ofFbo::Settings s;
    s.width = bubbleWidth + FBO_SAFETY_MARGIN;
    s.height = bubbleHeight + FBO_SAFETY_MARGIN;
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;

    gpuBlur.setup(s, false);
    gpuBlur.blurOffset = 18;
    gpuBlur.blurPasses = 26;
    gpuBlur.numBlurOverlays = 1;
    gpuBlur.blurOverlayGain = 255;
}

void Tweet::update() {
    if (alpha == 0 && fade == true) {
        dead = true;
    }

    colors = getTweetColor();

    if ((location.y + bubbleHeight * 2) < 0) {
      dead = true;
    }

    auto durationY = 10.f;
    auto durationX = 3.f;
    auto endTimeY = initTime + durationY;
    auto endTimeX = initTime + durationX;
    auto now = ofGetElapsedTimef();

    if (initTime + 12.f < now) {
        fade = true;
    }

    location.y = ofxeasing::map(now, initTime, endTimeY, initLocationY, endLocationY, &ofxeasing::linear::easeInOut);

    if (now < endTimeX) {
        location.x = ofxeasing::map(now, initTime, endTimeX, initLocationX, endLocationX, &ofxeasing::quint::easeOut);
    } else {
        location.x += ofMap(ofNoise(movementNoiseSeed), 0, 1, -8, 8);
        movementNoiseSeed += 0.03;
    }

    // adding noise to the bubble
    for (int i = 0; i<ellipsePoints.size(); i++){
        noiseSeeds[i]+=0.04;
        newVec = originEllipsePoints[i] + normVectors[i] * ofNoise(noiseSeeds[i]) * noiseStep;
        ellipsePoints[i] = newVec;
    }

    location.x = ofClamp(location.x, minX, maxX);

//    gpuBlur.blurOffset = 100 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0, true);
//    gpuBlur.blurPasses = 50 * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1, true);
//    cout << "Y " << 100 * ofMap(ofGetMouseY(), 0, ofGetHeight(), 1, 0, true) << endl;
//    cout << "X " << 50 * ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1, true) << endl;
}

void Tweet::draw() {
    if (ellipsePoints.size()>0) {
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(location);

        //BLUR
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        gpuBlur.beginDrawScene();
        ofClear(0, 0, 0, 0);
        // BUBBLE STUFF
        ofPushMatrix();
        ofTranslate((bubbleWidth + FBO_SAFETY_MARGIN)/2, (bubbleHeight + FBO_SAFETY_MARGIN)/2);
        ofPushMatrix();
        ofTranslate(SHADOW_OFFSET, SHADOW_OFFSET);
        ofSetColor(colors.shadowColor);
        drawBubble();
        ofPopMatrix();
        ofSetColor(colors.bubbleColor);
        drawBubble();
        ofPopMatrix();
        gpuBlur.endDrawScene();
        //blur the fbo
        //blending will be disabled at this stage
        gpuBlur.performBlur();
        //draw the "clean" scene
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        //gpuBlur.drawSceneFBO();
        //overlay the blur on top
        //ofTranslate(mouseX, mouseY);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
        ofPushMatrix();
        ofTranslate(-(bubbleWidth + FBO_SAFETY_MARGIN)/2, -(bubbleHeight + FBO_SAFETY_MARGIN)/2);
        gpuBlur.drawBlurFbo();
        ofPopMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        // STRING STUFF
        if (!wrappedString.empty()) {
            ofSetColor(colors.textColor);

            int locX = -(stringBox.width / 2) + ((BIRD_SIZE + BIRD_PADDING) / 2);
            int locY = -(stringBox.height - font->getLineHeight() - BUBBLE_TOP_LEFT_SHIFT ) / 2;

            font->drawString(wrappedString, locX, locY);
            ofSetColor(colors.authorColor);
            font->drawString(author, locX, locY);
        }
        ofPopStyle();

        if (abs(stringBox.height - font->getLineHeight()) < 5) {
            ofTranslate(0, -30);
        }
        
        if (abs(stringBox.height - font->getLineHeight() * 2) < 5) {
            ofTranslate(0, -15);
        }
        
        ofPushStyle();
        ofSetColor(colors.imageColor);
        profileImage.draw(ofPoint(-((stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2), -stringBox.height /2), 48, 48);

        // BIRD STUFF
        ofSetColor(colors.birdColor);
        ofFill();
        bird.draw( ofPoint(-((stringBox.width + BIRD_SIZE + BIRD_PADDING) / 2), -stringBox.height / 2 + 60));// , 50, 40.65);
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
    colors.textColor = ofColor(50,0,0,alpha);
    colors.authorColor = ofColor(0,0,0,alpha);
    colors.birdColor = ofColor(255,255,255);
    colors.shadowColor = ofColor(50, alpha*0.85);
    colors.imageColor = ofColor(255, alpha);

    if (moodLevel < 0) {
        colors.bubbleColor.set(255,255,255,alpha);
        colors.textColor.set(50,0,0,alpha);
        colors.authorColor = ofColor(0,0,0,alpha);
        colors.birdColor.set(200,0,0,alpha);
        colors.shadowColor.set(50, alpha*0.85);
    } else if (moodLevel > 0) {
        colors.bubbleColor.set(255,255,255,alpha);
        colors.birdColor.set(0,200,0,alpha);
        colors.shadowColor.set(50, alpha*0.85);
    }

    if (fade==true) {
        alpha-=10;
    } else {
        alpha+=10;
    }
    alpha=ofClamp(alpha,0,255);
    return colors;
}
