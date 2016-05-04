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
    Tweet();
    void age();
    void setup( ofPoint _location, string tweetContent, string tweetAuthor, int _moodLevel, int paddingWidth, int _paddingHeight);
    void draw();
    void update();
    int getTtl();
    struct TweetColors {
        ofColor bgColor;
        ofColor textColor;
        ofColor birdColor;
    };

    TweetColors getTweetColor(int alpha);
    TweetColors colors;
    
    float initTime;
    int initLocation;
    int endLocation;
    string text;
    string author;
    //ofImage profileImage;
    int moodLevel;
    Boolean display;
    ofTrueTypeFont font;
    int paddingHeight;
    ofPoint location;
    string wrappedString;
    ofRectangle stringBox;
    int alpha;
    ofImage bird;
    
    //bubble stuff
    float noiseSeed;
    int totalRays;
    float angleStep;
    float stepSize;
    vector <ofPoint> bubblePoints;
    vector <float> radii;
    int ellipseWidthRad, ellipseHeightRad;

    
private:
    int ttl;
};

#endif /* Tweet_hpp */
