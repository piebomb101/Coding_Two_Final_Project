#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ncKinectv2Core.h"
#include "ncKinectCamera.h"
#include "ncKinectUserManager.h"
#include "ncKinect3dScene.h"

//OSC PORT
#define PORT 8000


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void audioIn(ofSoundBuffer& input);

		void checkerLX(float LXpos);
		void checkerLY(float LXpos);
		void checkerRX(float LXpos);
		void checkerRY(float LXpos);



		//OSC CALLS
		ofxOscSender oscSend;
		
		ofxOscReceiver osc;

		

		int winW = ofWindowSettings().getWidth();
		int winH = ofWindowSettings().getHeight();

		//CONSTALATION MESH
		ofMesh mesh;

		//CONSTALATIONAL POS VALUES
		//Values to initilise only

		//Number of points
		float uiAmount =50;

		//Left hand mesh x pos
		float uiPos1 = 1000;

		//Right hand mesh x pos
		float uiPos2 = -1000;

		//mesh Y max/min
		float uiPos3 = 500;
		float uiPos4= -500;

		//mesh Z max/min
		float uiPos5 = -500;
		float uiPos6 = 500;

		//Distance at which line of vertex is drawn/alpha
		float uiDistance = 400;

		//draw points on/off
		bool uiPoints = 1;

		int rotate = 0;

		//AUDIO ANALYSIS VARS
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;

		int 	bufferCounter;
		int 	drawCounter;
		float smoothedVol;
		float scaledVol;

		ofSoundPlayer sound;
		ofSoundStream soundStream;

		float* fft;
		float* soundSpectrum;
		int bands;

		//KINECT Calls
		ncKinectv2Core kinectcore;
		nCKinectCamera kinectcamera;
		ncKinectUserManager kinectusermanager;
		ncKinect3dScene kinectscene;

		ofImage bodyIndexImg;
		ofImage bodyIndexImgColor;

		ofImage depthmapimage;

		ofImage colorimage;

		void newUser(NCGenericEventArg& arg);
		void lostUser(NCGenericEventArg& arg);

		bool fs;

		
};
