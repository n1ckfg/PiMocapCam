#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
//#include "TerminalListener.h"
#include "ofxOsc.h"
#define HOST "Allosaurus.local"
#define PORT 7110

class testApp : public ofBaseApp {

    public:
        void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
	    //void testApp::onCharacterReceived(KeyListenerEventData& e);		
		//TerminalListener consoleListener;
        string getHostName();

		ofxCvPiCam cam;
		cv::Mat frame, frameProcessed;
		
        int width;
		int height;
		int fps = 60;
	    int thresholdValue = 127;
		int thresholdKeyCounter = 0;
		bool thresholdKeyFast = false;
		bool doDrawInfo = true;

		string oscAddress = "blob";
		ofxOscSender sender;
		void sendOsc(int index, float x, float y, float z);

	    ofxCv::ContourFinder contourFinder;
		float contourThreshold = 2.0; //127.0;
		float contourMin = 1.0; //10.0;
		float contourMax = 250.0; //150.0;
		ofxCv::TrackingColorMode trackingColorMode;
		ofColor targetColor;

		ofFile file;
		ofBuffer buff;
		string hostname = "RPi";

};
