# emotional-mirror-frontend

To run on Ubuntu

* Install a fresh copy of Ubuntu (tested on 14.04)
* Install backend as per instructions in random-quark/emotional-mirror-backend
* Install openFrameworks 0.9.3
* Install needed addons
  * ofxCv
  * ofxEasing
  * ofxFaceTracker
  * ofxGui
  * ofxJSON
  * ofxOpenCv
  * ofxTrueTypeFontUC
* git clone https://github.com/random-quark/emotional-mirror-frontend.git (into your of/apps/myApps directory)
* cd emotional-mirror-frontend
* make
* make RunRelease
* install supervisord
* setup supervisord config
* add xrandr --output HDMI1 --rotate right to startup applications
* add supervisord to startup applications (ensure it's alphabetically after previous command)
* edit ./bin/data/settings.xml and set absolute_path to the directory in emotional-mirror-backend where profile images are saved by spider
