#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppNoWindow.h"

int main() {
	//ofAppNoWindow w;
	//ofSetupOpenGL(&w, 640, 480, OF_WINDOW);
	//ofSetupOpenGL(640, 480, OF_WINDOW);
	ofSetupOpenGL(OF_FULLSCREEN);
	ofRunApp(new ofApp());
}
