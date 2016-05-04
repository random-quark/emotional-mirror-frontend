//
//  Util.cpp
//  new-emotion-mirror copy
//
//  Created by Tom Chambers on 12/04/2016.
//
//

#include "Util.hpp"

Util::Util() {
    
}

string Util::wrapString(string text, int maxLineWidth, ofTrueTypeFont font) {
    int timesCalled=0;
    string output = "";
    string currentLine = "";
    vector <string> words = ofSplitString(text, " ");
    
    for(int i=0; i<words.size(); i++) {
        
        string currentWord = words[i];
        
        currentLine += currentWord + " ";
        int currentLineWidth = font.stringWidth(currentLine);
        if(currentLineWidth >= maxLineWidth) {
            currentLine = currentWord;
            output += "\n";
        }
        
        output += currentWord + " ";
    }
    
    return output;
    
}