//
//  Tweet.hpp
//  emotion-mirror-4
//
//  Created by Tom Chambers on 18/03/2016.
//
//

using namespace std;

#ifndef Tweet_hpp
#define Tweet_hpp

#include <stdio.h>
#include <string>
#include <ofMath.h>
#include <ofImage.h>
#include <ofGraphics.h>
#include <ofTrueTypeFont.h>

#define HEIGHT 900

class Tweet {
public:
    float initTime;
    int initLocation;
    int endLocation;
    
    string text;
    string author;
    ofImage profileImage;
    int timestamp;
    int moodLevel;

    Boolean display;
    ofTrueTypeFont font;
    
    void age();
    void draw();
    void update();
    int getTtl();
    
    int paddingHeight;
    
    ofPoint location;
    
    float centerX, centerY;
    std::vector <float> x;
    std::vector <float> y;
    float stepSize;
    int formResolution, initRadiusWidth, initRadiusHeight;
    string wrappedString;
    ofRectangle stringBox;

    ofPoint velocity;
    ofPoint force;
    
    float drag;
    int minDist;
    
    int alpha;
    
    struct TweetColors {
        ofColor bgColor;
        ofColor textColor;
        ofColor birdColor;
    };
    
    int totalRays;
    int radius;
    float angleStep;
    vector<float> noiseSeeds;
    
    ofImage bird;
    
    Tweet(ofTrueTypeFont font, ofPoint location, string text, string author, string profileImageUrl, int moodLevel, int timestamp, int paddingWidth, int paddingHeight);
    
private:
    int ttl;
};

#endif /* Tweet_hpp */
