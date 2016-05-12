//
//  Util.cpp
//  new-emotion-mirror copy
//
//  Created by Random Quark on 18/03/2016.
//  randomQuark.com
//

#include "Util.hpp"

Util::Util() {
    
}

string Util::wrapString(string text, int maxLineWidth, ofxTrueTypeFontUC* font) {
    int timesCalled=0;
    string output = "";
    string currentLine = "";
    vector <string> words = ofSplitString(text, " ");
    
    for(int i=0; i<words.size(); i++) {
        
        string currentWord = words[i];
        
        currentLine += currentWord + " ";
        int currentLineWidth = font->stringWidth(currentLine);
        if(currentLineWidth >= maxLineWidth) {
            currentLine = currentWord;
            output += "\n";
        }
        
        output += currentWord + " ";
    }
    
    return output;
    
}