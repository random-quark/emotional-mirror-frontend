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
    void setup(ofxTrueTypeFontUC* _font_bigger, ofxTrueTypeFontUC* _font_smaller, ofPoint _location, string tweetContent, string tweetHandle, string tweetAuthor, string profileImageUrl, float _moodLevel);
    void draw();
    void update();
    int getTtl();
    struct TweetColors {
        ofColor bubbleColor;
        ofColor shadowColor;
        ofColor textColor;
        ofColor birdColor;
        ofColor authorColor;
        ofColor handleColor;
        ofColor imageColor;
    };

    TweetColors getTweetColor();
    TweetColors colors;

    float initTime;
    string text;
    string tweetAuthor;
    string tweetHandle;
    ofImage profileImage;
    float moodLevel;
    bool fade;
    bool dead;
    ofxTrueTypeFontUC* font_bigger, *font_smaller;
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
