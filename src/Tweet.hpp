//
//  Tweet.hpp
//  emotion-mirror-4
//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
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
#include "ofxTrueTypeFontUC.h"
#include "ofxFboBlur.h"

#define HEIGHT 900

class Tweet {
public:
    Tweet();
    void age();
    void setup(ofxTrueTypeFontUC* _font, ofPoint _location, string tweetContent, string tweetAuthor, string profileImageUrl, float _moodLevel);
    void draw();
    void update();
    int getTtl();
    struct TweetColors {
        ofColor bubbleColor;
        ofColor shadowColor;
        ofColor textColor;
        ofColor birdColor;
        ofColor authorColor;
    };

    TweetColors getTweetColor();
    TweetColors colors;

    float initTime;
    string text;
    string author;
    ofImage profileImage;
    float moodLevel;
    bool fade;
    bool dead;
    ofxTrueTypeFontUC* font;
    ofPoint location;
    string wrappedString;
    ofRectangle stringBox;
    int alpha;
    ofImage bird;
    int initLocationY;
    int initLocationX;
    int endLocationY;
    int endLocationX;
    float movementNoiseSeed;
    int minX;
    int maxX;

    //bubble stuff
    vector <float> noiseSeeds;
    vector <ofPoint> ellipsePoints, originEllipsePoints, normVectors;
    int cornerRadius;
    int resamplingRate;
    int noiseStep;
    int bubbleWidth, bubbleHeight;
    ofPath path;
    ofPoint newVec;
    void drawBubble();
    ofxFboBlur gpuBlur;

private:
    int ttl;
};

#endif /* Tweet_hpp */
