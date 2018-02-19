#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
//#include "TerminalListener.h"
#include "ofxOsc.h"
#define HOST "255.255.255.255"
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
        //string getHostName();

		string hostName;
		ofFile file;
		ofBuffer buff;

		ofxCvPiCam cam;
		cv::Mat frame, frameProcessed;
		
		string oscAddress;
        int width;
		int height;
	    int thresholdValue;
		int thresholdKeyCounter;
		bool thresholdKeyFast;
		bool doDrawInfo;

		ofxOscSender sender;
		void sendOsc(int index, float x, float y, float z);

	    ofxCv::ContourFinder contourFinder;
		float contourThreshold;
		ofxCv::TrackingColorMode trackingColorMode;
		ofColor targetColor;

};
