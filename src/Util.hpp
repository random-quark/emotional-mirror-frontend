//
//  Util.hpp
//  new-emotion-mirror copy
//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
//

#ifndef Util_hpp
#define Util_hpp

#include <stdio.h>
#include <string>
#include <ofxTrueTypeFontUC.h>
#include <ofTrueTypeFont.h>
#include <vector>

using namespace std;

class Util {
public:
    Util();
    static string wrapString(string text, int width, ofxTrueTypeFontUC* font);
};


#endif /* Util_hpp */
