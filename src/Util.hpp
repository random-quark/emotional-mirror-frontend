//
//  Util.hpp
//  new-emotion-mirror copy
//
//  Created by Tom Chambers on 12/04/2016.
//
//

#ifndef Util_hpp
#define Util_hpp

#include <stdio.h>
#include <string>
#include <ofTrueTypeFont.h>
#include <vector>

using namespace std;

class Util {
public:
    Util();
    static string wrapString(string text, int width, ofTrueTypeFont font);
};


#endif /* Util_hpp */
