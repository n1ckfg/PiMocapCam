#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

int main() {
	ofAppNoWindow w;
	ofSetupOpenGL(&w, 640, 480, OF_WINDOW);
	ofRunApp(new ofApp());
}
