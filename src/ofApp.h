#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {

    public:
        void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);

		string compname;
		string host;
		int port;

		bool debug;
		bool video;
		bool brightestPixel;
		bool blobs;
		bool contours;

		ofFile file;
		ofBuffer buff;
		ofBuffer txBuffer;
		ofxXmlSettings settings;

		ofxCvPiCam cam;
		cv::Mat frame, frameProcessed;
		ofImage gray;
		int videoQuality; // 5 best to 1 worst, default 3 medium

		// for more settings, see:
		// https://github.com/orgicus/ofxCvPiCam/blob/master/example-ofxCvPiCam-allSettings/src/testApp.cpp

        int camShutterSpeed; // 0 to 330000 in microseconds, default 0
    	int camSharpness; // -100 to 100, default 0
    	int camContrast; // -100 to 100, default 0
    	int camBrightness; // 0 to 100, default 50
		int camIso; // 100 to 800, default 300
		int camExposureCompensation; /// -10 to 10, default 0;
		// 0 off, 1 auto, 2 night, 3 night preview, 4 backlight, 5 spotlight, 6 sports, 7, snow, 8 beach, 9 very long, 10 fixed fps, 11 antishake, 12 fireworks, 13 max
		int camExposureMode; // 0 to 13, default 0

		//string oscAddress;
        int width;
		int height;
	    int thresholdValue;
		int thresholdKeyCounter;
		bool thresholdKeyFast;
		//bool doDrawInfo;

		ofxOscSender sender;
		void sendOscVideo();
		void sendOscBlobs(int index, float x, float y);
		void sendOscContours(int index, float x, float y);
		void sendOscPixel(float x, float y);

	    ofxCv::ContourFinder contourFinder;
		float contourThreshold;
		ofxCv::TrackingColorMode trackingColorMode;
		ofColor targetColor;

};
