# emotional-mirror-frontend

To run on Ubuntu

* Install a fresh copy of Ubuntu (tested on 14.04)
* Install backend as per instructions in random-quark/emotional-mirror-backend
* Stuff to do on Ubuntu machine to make sure it run smoothly, see: emotional-mirror-backend/COMPUTER_SETUP.md
* Install openFrameworks 0.9.3
* Install needed addons
  * ofxCv
  * ofxEasing
  * ofxFaceTracker
  * ofxGui
  * ofxJSON
  * ofxOpenCv
  * ofxTrueTypeFontUC
  * ofxXmlSettings
* git clone https://github.com/random-quark/emotional-mirror-frontend.git (into your of/apps/myApps directory)
* cd emotional-mirror-frontend
* make
* make RunRelease
* edit ./bin/data/settings.xml and set absolute_path to the directory in emotional-mirror-backend where profile images are saved by spider

* in the ofxFaceTracker go to `ExpressionClassifier` class and the `load(string directory)` function and add this line `dir.sort();` immediately after the `ofDirectory dir(directory);` line. This is to short the expressions alphabetically.
