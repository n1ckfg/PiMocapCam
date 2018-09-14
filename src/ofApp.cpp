#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    settings.loadFile("settings.xml");

    oscAddress = "blob";
    doDrawInfo  = true;
    width = ofGetWidth();
    height = ofGetHeight();
    thresholdKeyCounter = 0;
    thresholdKeyFast = false;    

    cam.setup(width, height, false); // color/gray;
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T) 0); // 0 = off, 1 = auto
    //cam.setFrameRate // not implemented in ofxCvPiCam

    ofSetVerticalSync(false);    
    ofSetFrameRate(90);
    host = settings.getValue("settings:host", ""); // hostname;
    port = settings.getValue("settings:port", 0); // default 7110;
    thresholdValue = settings.getValue("settings:threshold", 0); // default 127;
    contourThreshold = 2.0; // default 127.0;

    sender.setup(host, port);

    contourFinder.setMinAreaRadius(1); // default 10;
    contourFinder.setMaxAreaRadius(250); // default 150;
    //contourFinder.setInvert(true); // find black instead of white
    trackingColorMode = TRACK_COLOR_RGB;

    compname = "RPi";
    
    file.open(ofToDataPath("compname.txt"), ofFile::ReadWrite, false);
    if (file) {
        buff = file.readToBuffer();
        compname = buff.getText();
    } else {
        compname += "_" + ofGetTimestampString("%y-%m-%d-%H-%M-%S-%i");
        ofStringReplace(compname, "-", "");
        ofStringReplace(compname, "\n", "");
        ofStringReplace(compname, "\r", "");
        buff.set(compname.c_str(), compname.size());
        ofBufferToFile("compname.txt", buff);
    }
    cout << compname;
}

void ofApp::update() {
    frame = cam.grab();

    if(!frame.empty()) {
        //autothreshold(frameProcessed);        
        threshold(frame, frameProcessed, thresholdValue, 255, 0);    
        contourFinder.setThreshold(contourThreshold);
        contourFinder.findContours(frameProcessed);
    }
}

void ofApp::draw() {
    ofSetColor(255);
    if(!frame.empty()){
        drawMat(frameProcessed,0,0);

        ofSetLineWidth(2);
        //contourFinder.draw();

        ofNoFill();
        int n = contourFinder.size();
        for(int i = 0; i < n; i++) {
            ofSetColor(cyanPrint);
            float circleRadius;
            ofVec2f circleCenter = toOf(contourFinder.getMinEnclosingCircle(i, circleRadius));
            ofCircle(circleCenter, circleRadius);
            ofCircle(circleCenter, 1);

            sendOsc(i, circleCenter.x, circleCenter.y);
        }
    }

    if (doDrawInfo) {
        stringstream info;
        info << "FPS: " << ofGetFrameRate() << "\n";
        //info << "Camera Resolution: " << cam.width << "x" << cam.height << " @ "<< "xx" <<"FPS"<< "\n";
        ofDrawBitmapStringHighlight(info.str(), 10, 10, ofColor::black, ofColor::yellow);
    }
   
}

void ofApp::sendOsc(int index, float x, float y) {
    ofxOscMessage m;
    m.setAddress("/" + oscAddress);
    m.addStringArg(compname);
    m.addIntArg(index);
    m.addFloatArg(x / (float) width);
    m.addFloatArg(y / (float) height);
    sender.sendMessage(m);
}

void ofApp::keyPressed(int key) {
    thresholdKeyCounter++;
    if (thresholdKeyCounter > 10) thresholdKeyFast = true;

    if (key == OF_KEY_PAGE_DOWN || key == OF_KEY_DOWN) {
        if (thresholdKeyFast) {
            thresholdValue -= 10;
        } else {
            thresholdValue -= 1;
        }
    }

    if (key == OF_KEY_PAGE_UP || OF_KEY_UP) {
       if (thresholdKeyFast) {
           thresholdValue += 10;
       } else {
           thresholdValue += 1;
       }
    }

    if (thresholdValue < 0) thresholdValue = 0;
    if (thresholdValue > 255) thresholdValue = 255;
}

void ofApp::keyReleased(int key) {
    thresholdKeyFast = false;
    thresholdKeyCounter = 0;
}
