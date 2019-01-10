#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    settings.loadFile("settings.xml");

    //doDrawInfo  = true;
    width = ofGetWidth();
    height = ofGetHeight();
    thresholdKeyCounter = 0;
    thresholdKeyFast = false;    
    ofSetVerticalSync(false);    
    framerate = settings.getValue("settings:framerate", 0); 
    ofSetFrameRate(framerate);

    videoQuality = settings.getValue("settings:video_quality", 0); 
    videoColor = (bool) settings.getValue("settings:video_color", 0); 

    if (videoColor) {
        gray.allocate(width, height, OF_IMAGE_COLOR);
    } else {
        gray.allocate(width, height, OF_IMAGE_GRAYSCALE);        
    }
    cam.setup(width, height, videoColor); // color/gray;

    camSharpness = settings.getValue("settings:sharpness", 0); 
    camContrast = settings.getValue("settings:contrast", 0); 
    camBrightness = settings.getValue("settings:brightness", 0); 
    camIso = settings.getValue("settings:iso", 0); 
    camExposureMode = settings.getValue("settings:exposure_mode", 0); 
    camExposureCompensation = settings.getValue("settings:exposure_compensation", 0); 
    camShutterSpeed = settings.getValue("settings:shutter_speed", 0);

    cam.setSharpness(camSharpness);
    cam.setContrast(camContrast);
    cam.setBrightness(camBrightness);
    cam.setISO(camIso);
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T) camExposureMode);
    cam.setExposureCompensation(camExposureCompensation);
    cam.setShutterSpeed(camShutterSpeed);
    //cam.setFrameRate // not implemented in ofxCvPiCam

    host = settings.getValue("settings:host", ""); 
    port = settings.getValue("settings:port", 0);
    thresholdValue = settings.getValue("settings:threshold", 0); 
    
    debug = (bool) settings.getValue("settings:debug", 0);
    video = (bool) settings.getValue("settings:video", 0); 
    blobs = (bool) settings.getValue("settings:blobs", 0);
    contours = (bool) settings.getValue("settings:contours", 0); 
    brightestPixel = (bool) settings.getValue("settings:brightest_pixel", 0); 

    contourThreshold = 2.0;
    contourMinAreaRadius = 1.0;
    contourMaxAreaRadius = 250.0;

    sender.setup(host, port);

    contourFinder.setMinAreaRadius(contourMinAreaRadius);
    contourFinder.setMaxAreaRadius(contourMaxAreaRadius);
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

    if (!frame.empty()) {
    	if (video) {
            toOf(frame, gray.getPixelsRef());
            switch(videoQuality) {
                case 5:
                    ofSaveImage(gray, txBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
                    break;
                case 4:
                    ofSaveImage(gray, txBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
                    break;
                case 3:
                    ofSaveImage(gray, txBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
                    break;
                case 2:
                    ofSaveImage(gray, txBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
                    break;
                case 1:
                    ofSaveImage(gray, txBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
                    break;
            }
       	}

    	if (blobs || contours) {
	        //autothreshold(frameProcessed);        
	        threshold(frame, frameProcessed, thresholdValue, 255, 0);    
	        contourFinder.setThreshold(contourThreshold);
	        contourFinder.findContours(frameProcessed);
    	}

    	if (brightestPixel) {
    		toOf(frame, gray.getPixelsRef());
    	}
    }
}

void ofApp::draw() {
    ofSetColor(255);
    ofBackground(0);
    
    if(!frame.empty()) {
        if (debug) {
            if (video && !blobs && !contours) {
                drawMat(frame, 0, 0);
            } else if (!video && (blobs || contours)) {
                drawMat(frameProcessed, 0, 0);
            }
        }

        if (video) {
            sendOscVideo();
        } 

        if (blobs) {
            if (debug) {
            	ofSetLineWidth(2);
            	//contourFinder.draw();
            	ofNoFill();
            }
            int n = contourFinder.size();
            for (int i = 0; i < n; i++) {
                float circleRadius;
                ofVec2f circleCenter = toOf(contourFinder.getMinEnclosingCircle(i, circleRadius));
                if (debug) {
                	ofSetColor(cyanPrint);
                	ofCircle(circleCenter, circleRadius);
                	ofCircle(circleCenter, 1);
                }

                sendOscBlobs(i, circleCenter.x, circleCenter.y);
            }
        }

        if (contours) {
            if (debug) {
                ofSetLineWidth(2);
                contourFinder.draw();
                ofNoFill();
            }
            
            int n = contourFinder.size();
            for (int i = 0; i < n; i++) {
                string points;
                ofPolyline line = contourFinder.getPolyline(i);
                vector<ofPoint> cvPoints = line.getVertices();
                for(int i=0; i<cvPoints.size(); i++) {
                    char x[4];
                    x = (char[]) cvPoints[i].x;
                    char y[4];
                    y = (char[]) cvPoints[i].y;
                    points.push_back(x);
                    points.push_back(y);
                }
                contourBuffer.set(points);
                sendOscContours(i);
            }        
        }
           
        if (brightestPixel) {
        	// this mostly useful as a performance baseline
            // https://openframeworks.cc/ofBook/chapters/image_processing_computer_vision.html
            float maxBrightness = 0; 
            float maxBrightnessX = 0; 
            float maxBrightnessY = 0;
            int skip = 2;

            for (int y=0; y<height - skip; y += skip) {
                for (int x=0; x<width - skip; x += skip) {
                    ofColor colorAtXY = gray.getColor(x, y);
                    float brightnessOfColorAtXY = colorAtXY.getBrightness();
                    if (brightnessOfColorAtXY > maxBrightness && brightnessOfColorAtXY > thresholdValue) {
                        maxBrightness = brightnessOfColorAtXY;
                        maxBrightnessX = x;
                        maxBrightnessY = y;
                    }
                }
            }

            if (debug) {
            	ofNoFill();
            	ofVec2f circleCenter = ofVec2f(maxBrightnessX, maxBrightnessY);
            	ofCircle(circleCenter, 40);
            }

            sendOscPixel(maxBrightnessX, maxBrightnessY);
        }
    }

    if (debug) {
        stringstream info;
        info << "FPS: " << ofGetFrameRate() << "\n";
        //info << "Camera Resolution: " << cam.width << "x" << cam.height << " @ "<< "xx" <<"FPS"<< "\n";
        ofDrawBitmapStringHighlight(info.str(), 10, 10, ofColor::black, ofColor::yellow);
    }
   
}

void ofApp::sendOscVideo() {
    ofxOscMessage m;
    m.setAddress("/video");
    m.addStringArg(compname);    
    
    m.addBlobArg(txBuffer);
    
    sender.sendMessage(m);
}

void ofApp::sendOscBlobs(int index, float x, float y) {
    ofxOscMessage m;
    m.setAddress("/blob");
    m.addStringArg(compname);
    
    m.addIntArg(index);
    m.addFloatArg(x / (float) width);
    m.addFloatArg(y / (float) height);

    sender.sendMessage(m);
}

void ofApp::sendOscContours(int index) {
    ofxOscMessage m;
    m.setAddress("/contour");
    m.addStringArg(compname);
    
    m.addIntArg(index);
    m.addBlobArg(contourBuffer);

    sender.sendMessage(m);
}

void ofApp::sendOscPixel(float x, float y) {
    ofxOscMessage m;
    m.setAddress("/pixel");
    m.addStringArg(compname);
    
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
