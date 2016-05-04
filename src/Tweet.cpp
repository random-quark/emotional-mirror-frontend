//
//  Tweet.cpp
//  emotion-mirror-4
//
//  Created by Tom Chambers on 18/03/2016.
//
//

#include "Tweet.hpp"
#include <ofTrueTypeFont.h>
#include "Util.hpp"

Tweet::TweetColors getTweetColor(Tweet tweet, int alpha) {
    ofColor bgColor = ofColor(255,255,255,alpha);
    ofColor textColor = ofColor(0);
    
    if (tweet.moodLevel == 3) {
        bgColor.set(255,255,255,alpha);
        textColor.set(0,0,0,alpha);
    } else if (tweet.moodLevel == 1) {
        bgColor.set(255,255,255,alpha);
    } else {
        bgColor.set(255,255,255,alpha);
    }
    Tweet::TweetColors tweetColors;
    tweetColors.bgColor = bgColor;
    tweetColors.textColor = textColor;
    return tweetColors;
}

Tweet::Tweet(){

}

void Tweet::setup(ofTrueTypeFont _font, ofPoint _location, string tweetContent, string tweetAuthor, string profileImageUrl, int _moodLevel, int _timestamp, int paddingWidth, int _paddingHeight) {
    font = _font;
    location = _location;
    profileImage.load(profileImageUrl);
    moodLevel = _moodLevel;
    timestamp = _timestamp;
    display = true;
    paddingHeight = _paddingHeight;
    
    text = "@" + tweetAuthor + " " + tweetContent;
    
    formResolution = 15;
    
    wrappedString = Util::wrapString(text, 350, font);
    stringBox = font.getStringBoundingBox(wrappedString,0,0);
    
    int width = stringBox.width + paddingWidth;
    int height = stringBox.height + paddingHeight;
    
    stepSize = 0.5;
    float widthHypotenuse = sqrt(width*width + width*width);
    float heightHypotenuse = sqrt(height*height + height*height);
    initRadiusWidth = widthHypotenuse / 2;
    initRadiusHeight = heightHypotenuse / 2;
    
    centerX = width / 2;
    centerY = height / 2;
    
//    float angleStep = 360.0/formResolution;
    angleStep = 360.0/formResolution;
    
    for (int i=0; i<formResolution; i++){
        x.push_back(cos(ofDegToRad(angleStep*i)) * initRadiusWidth);
        y.push_back(sin(ofDegToRad(angleStep*i)) * initRadiusHeight);
    }
    
    drag = ofRandom(0.95, 0.998);
    minDist = 50;
    
    alpha = 0;
    
    //new noisy sun
    int totalRays = 100;
    int radius = 300;
    //    float angleStep = 360.0 / totalRays;
    
    for (int x=0; x<totalRays; x++)
    {
        noiseSeeds.push_back(ofRandom(10000));
    }
}

void Tweet::update() {
    for (int i=0; i<formResolution; i++){
        x[i] += ofRandom(-stepSize,stepSize);
        y[i] += ofRandom(-stepSize,stepSize);
    }
    
    if (alpha < 255) {
        alpha+=20;
        if (alpha > 255) {
            alpha = 255;
        }
    }
    
    if (location.y + initRadiusHeight < 0) {
        cout << "location y gone " << location.y << endl;
        display = false;
    }
    
//    location.y -= 15;
//    if (location.y - initRadiusHeight > HEIGHT) {
//        display = false;
//    }
}

void Tweet::draw() {
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(location.x, location.y);
    
    TweetColors colors = getTweetColor(*this, alpha);
    
    ofSetColor(colors.bgColor);
    ofFill();
    
    ofBeginShape();
    for (int i=0; i<totalRays; i++)
    {
        float noisedRadius = radius + (ofNoise(noiseSeeds[i]) * stepSize);
        float endX = sin(ofDegToRad(i*angleStep)) * noisedRadius;
        float endY = cos(ofDegToRad(i*angleStep)) * noisedRadius;
        noiseSeeds[i]+=0.01;
        
        ofVertex(endX, endY);
    }
    ofEndShape();
    
//    ofBeginShape();
//    ofCurveVertex(x[formResolution-1]+centerX, y[formResolution-1]+centerY);
//    for (int i=0; i<formResolution; i++){
//        ofCurveVertex(x[i]+centerX, y[i]+centerY);
//    }
//    ofCurveVertex(x[0]+centerX, y[0]+centerY);
//    ofCurveVertex(x[1]+centerX, y[1]+centerY);
//    ofEndShape();
    
    if (!wrappedString.empty()) {
        ofSetColor(colors.textColor);
        
        font.drawString(wrappedString, 0, font.getLineHeight() + (paddingHeight / 4));
    }
    
    ofPopMatrix();
    ofPopStyle();
}
