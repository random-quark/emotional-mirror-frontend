#include "ofApp.h"
#include "ofAppGlutWindow.h"

#define TWEET_LIFESPAN 300

int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640, 480, OF_FULLSCREEN);
	ofRunApp(new ofApp());
}
