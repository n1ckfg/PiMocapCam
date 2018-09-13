#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#define HOST "Allosaurus.local"
#define PORT 7110

class ofApp : public ofBaseApp {

    public:
        void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);

		string hostName;
		ofFile file;
		ofBuffer buff;
		bool settingsLoaded;
		ofxXmlSettings settings;

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
		void sendOsc(int index, float x, float y);

	    ofxCv::ContourFinder contourFinder;
		float contourThreshold;
		ofxCv::TrackingColorMode trackingColorMode;
		ofColor targetColor;

};
