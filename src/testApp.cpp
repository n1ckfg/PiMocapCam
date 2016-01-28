#include "testApp.h"

using namespace cv;
using namespace ofxCv;

void testApp::setup() {
    doDrawInfo  = true;
    width = ofGetWidth();
    height = ofGetHeight();
    thresholdKeyCounter = 0;
    thresholdKeyFast = false;    

    cam.setup(width, height, false); //(w, h, color/gray);
    //cam.setISO(300); // 100 - 800
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T) 0); // 0 = off, 1 = auto
    // * * *
    //cam.setFrameRate ???
    // * * *

    //ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetLogLevel("ofThread", OF_LOG_ERROR);
    ofSetVerticalSync(false);    

    //consoleListener.setup(this);

    ofSetFrameRate(60);
    thresholdValue = 127;
    contourThreshold = 127.0;

    sender.setup(HOST, PORT);

    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(150);
    //contourFinder.setInvert(true); // find black instead of white
    trackingColorMode = TRACK_COLOR_RGB;
}

void testApp::update() {
    frame = cam.grab();

    if(!frame.empty()) {
        //autothreshold(frameProcessed);        
        threshold(frame,frameProcessed,thresholdValue,255,0);    
        contourFinder.setThreshold(contourThreshold);
        contourFinder.findContours(frameProcessed);
    }
}

void testApp::draw() {
    ofSetColor(255);
    if(!frame.empty()){
        drawMat(frameProcessed,0,0);

        ofSetLineWidth(2);
        //contourFinder.draw();

        ofNoFill();
        int n = contourFinder.size();
        for(int i = 0; i < n; i++) {
            // smallest rectangle that fits the contour
            //ofSetColor(cyanPrint);
            //ofPolyline minAreRect = toOf(contourFinder.getMinAreaRect(i));
            //minAreRect.draw();

            // ellipse that best fits the contour
            //ofSetColor(magentaPrint);
            //cv::RotatedRect ellipse = contourFinder.getFitEllipse(i);
            //ofPushMatrix();
            //ofVec2f ellipseCenter = toOf(ellipse.center);
            //ofVec2f ellipseSize = toOf(ellipse.size);
            //ofTranslate(ellipseCenter.x, ellipseCenter.y);
            //ofRotate(ellipse.angle);
            //ofEllipse(0, 0, ellipseSize.x, ellipseSize.y);
            //ofPopMatrix();

            // minimum area circle that encloses the contour
            ofSetColor(cyanPrint);
            float circleRadius;
            ofVec2f circleCenter = toOf(contourFinder.getMinEnclosingCircle(i, circleRadius));
            ofCircle(circleCenter, circleRadius);
            ofCircle(circleCenter, 1);

            // convex hull of the contour
            //ofSetColor(yellowPrint);
            //ofPolyline convexHull = toOf(contourFinder.getConvexHull(i));
            //convexHull.draw();

            // defects of the convex hull
            //vector<cv::Vec4i> defects = contourFinder.getConvexityDefects(i);
            //for(int j = 0; j < defects.size(); j++) {
            //    ofLine(defects[j][0], defects[j][1], defects[j][2], defects[j][3]);
            //}

            // some different styles of contour centers
            //ofVec2f centroid = toOf(contourFinder.getCentroid(i));
            //ofVec2f average = toOf(contourFinder.getAverage(i));
            //ofVec2f center = toOf(contourFinder.getCenter(i));
            //ofSetColor(cyanPrint);
            //ofCircle(centroid, 1);
            //ofSetColor(magentaPrint);
            //ofCircle(average, 1);
            //ofSetColor(yellowPrint);
            //ofCircle(center, 1);

            // you can also get the area and perimeter using ofPolyline:
            // ofPolyline::getArea() and ofPolyline::getPerimeter()
            //double area = contourFinder.getContourArea(i);
            //double length = contourFinder.getArcLength(i);

            // balance is useful for detecting when a shape has an "arm" sticking out
            // if balance.length() is small, the shape is more symmetric: like I, O, X...
            // if balance.length() is large, the shape is less symmetric: like L, P, F...
            //ofVec2f balance = toOf(contourFinder.getBalance(i));
            //ofPushMatrix();
            //ofTranslate(centroid.x, centroid.y);
            //ofScale(5, 5);
            //ofLine(0, 0, balance.x, balance.y);
            //ofPopMatrix();

            sendOsc(i, circleCenter.x, circleCenter.y, circleRadius);
        }

        /*
        ofSetColor(255);
        drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 10);
        drawHighlightString(ofToString((int) contourThreshold) + " threshold", 10, 30);
        drawHighlightString(trackingColorMode == TRACK_COLOR_RGB ? "RGB tracking" : "hue tracking", 10, 50);

        ofTranslate(8, 75);
        ofFill();
        ofSetColor(0);
        ofRect(-3, -3, 64+6, 64+6);
        ofSetColor(targetColor);
        ofRect(0, 0, 64, 64);
        */
    }

    if (doDrawInfo) {
        stringstream info;
        info << "FPS: " << ofGetFrameRate() << "\n";
        //info << "Camera Resolution: " << cam.width << "x" << cam.height << " @ "<< "xx" <<"FPS"<< "\n";
        ofDrawBitmapStringHighlight(info.str(), 10, 10, ofColor::black, ofColor::yellow);
    }
   
}

void testApp::sendOsc(int index, float x, float y, float z) {
    ofxOscMessage m;
    m.setAddress("/blob");
    //m.addStringArg("hello");
    m.addIntArg(index);
    m.addFloatArg(x);
    m.addFloatArg(y);
    m.addFloatArg(z);
    sender.sendMessage(m);
}

void testApp::keyPressed(int key) {
    thresholdKeyCounter++;
    if (thresholdKeyCounter > 10) thresholdKeyFast = true;

    if (key == OF_KEY_PAGE_DOWN) {
        if (thresholdKeyFast) {
            thresholdValue -= 10;
        } else {
            thresholdValue -= 1;
        }
    }

    if (key == OF_KEY_PAGE_UP) {
       if (thresholdKeyFast) {
           thresholdValue += 10;
       } else {
           thresholdValue += 1;
       }
    }

    if (thresholdValue < 0) thresholdValue = 0;
    if (thresholdValue > 255) thresholdValue = 255;
}

void testApp::keyReleased(int key) {
    thresholdKeyFast = false;
    thresholdKeyCounter = 0;
}

//void testApp::onCharacterReceived(KeyListenerEventData& e) {
       //keyPressed((int) e.character);
//}
