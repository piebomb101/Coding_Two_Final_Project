#include "ofApp.h"

//TAKE HAND POSITION DATA (FILTERED TO REMOVE RANDOM ZEROS) AND SCALE TO DESIRED OSC RANGE
//THEN SEND AS OSC DATA
void ofApp::checkerLX(float LXpos) {
	float cleanLX;
	cleanLX = LXpos;
	float mapVal;
	mapVal = ofMap(cleanLX, 0, -2, 0, 1);

	cout << mapVal <<":::::::";

	ofxOscMessage oscLX;
	oscLX.setAddress("/ableton/1/LX");
	oscLX.addFloatArg(mapVal);
	oscSend.sendMessage(oscLX);
}

void ofApp::checkerLY(float LYpos) {
	float cleanLY;
	cleanLY = LYpos;
	float mapVal;
	float mapVal2;
	mapVal = ofMap(cleanLY, -0.5, 0.5, 0, 1);
	mapVal2 = ofMap(cleanLY, -0.5, 0.5, 0.3, 1);

	ofxOscMessage oscLY;
	oscLY.setAddress("/ableton/1/LY");
	oscLY.addFloatArg(mapVal);
	oscSend.sendMessage(oscLY);

	ofxOscMessage oscLY2;
	oscLY2.setAddress("/ableton/1/LY2");
	oscLY2.addFloatArg(mapVal2);
	oscSend.sendMessage(oscLY2);
}

void ofApp::checkerRX(float RXpos) {
	float cleanRX;
	cleanRX = RXpos;
	float mapVal1;
	float mapVal2;
	mapVal1 = ofMap(cleanRX, -0.5, 0.5, 0, 1);
	mapVal2 = ofMap(cleanRX, 0.5, -0.5, 0, 0.75);

	ofxOscMessage oscRX;
	oscRX.setAddress("/ableton/1/RX");
	oscRX.addFloatArg(mapVal1);
	oscSend.sendMessage(oscRX);

	ofxOscMessage oscRX2;
	oscRX2.setAddress("/ableton/1/RX2");
	oscRX2.addFloatArg(mapVal2);
	oscSend.sendMessage(oscRX2);
}

void ofApp::checkerRY(float RYpos) {
	float cleanRY;
	cleanRY = RYpos;
	float mapVal;
	mapVal = ofMap(cleanRY, -0.5, 0.5, 0, 1);
	float mapVal2;
	mapVal2 = ofMap(cleanRY, -0.5, 0.5, 0, 0.75);

	ofxOscMessage oscRY;
	oscRY.setAddress("/ableton/1/RY");
	oscRY.addFloatArg(mapVal); 
	oscSend.sendMessage(oscRY);  

	ofxOscMessage oscRY2;
	oscRY2.setAddress("/ableton/1/RY2");
	oscRY2.addFloatArg(mapVal2);
	oscSend.sendMessage(oscRY2);
}

//SETUP AUDIO IN FOR INTERFACE AND PERFORM RMS FOR AUDIO REACTIVITY
void ofApp::audioIn(ofSoundBuffer& input) {
	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++) {
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted += 2; 
	}

	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;

	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.09 * curVol;

	bufferCounter++;
}

//--------------------------------------------------------------
void ofApp::setup(){
	//OSC SETUP
	osc.setup(PORT);
	oscSend.setup("127.0.0.1", PORT);

	//VISUALS SETUP
	
	ofSetFrameRate(60);

	mesh.setMode(OF_PRIMITIVE_LINE_LOOP);

	//AUDIO SETUP
	
	//soundStream.printDeviceList();

	int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	bufferCounter = 0;
	drawCounter = 0;
	smoothedVol = 0.0;
	scaledVol = 0.0;

	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default
	//auto devices = soundStream.getDeviceList();
	//settings.device = devices[4];

	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	// settings.device = devices[0];

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name
	auto devices = soundStream.getMatchingDevices("Focusrite USB ASIO");
	if (!devices.empty()) {
		settings.setInDevice(devices[0]);
	}

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2; 
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

//KINECT SETUP
	kinectcore.setup(true);
	kinectcamera.setup();
	kinectusermanager.setup(kinectcore.getUsers(), kinectcamera);
	kinectscene.setup(kinectcore, kinectusermanager, kinectcamera);

	ofAddListener(ncKinectEventDispatcher::NCEVENTDISPATCHER.NEW_USER, this, &ofApp::newUser);
	ofAddListener(ncKinectEventDispatcher::NCEVENTDISPATCHER.LOST_USER, this, &ofApp::lostUser);

}

//--------------------------------------------------------------
void ofApp::update(){


	kinectcore.update(true, true, true);
	kinectcamera.update(kinectcore.getFloorPlane());
	kinectusermanager.update();

	//STORE XY COORDS OF EACH HAND IN VEC4
	ofVec4f handPosData = kinectcore.bothHands();

	//FILTER OUT ZERO VALUES, SEND EACH COORD AS ARGUMENT TO OSC FUNCTIONS
	//THEN MAP VALUES TO CONTROL VISUALS
	if (handPosData.x != 0.0) {
		
		checkerLX(handPosData.x);
		
		//LEFT HAND X CONTROLS LEFT SIDE OF MESH POSITION
		uiPos1 = ofMap(handPosData.x, -1, 1, -1000, 1000);
	}
	if (handPosData.y != 0.0) {
	
		checkerLY(handPosData.y);

		//LEFT HAND Y CONTROLS DISTANCE AT WHICH CONECTION BETWEEN POINTS IS MADE
		uiDistance = ofMap(handPosData.y, -1, 1, 50, 400);
	}
	if (handPosData.z != 0.0) {
		
		checkerRX(handPosData.z);
		//RIGHT HAND X CONTROLS RIGHT SIDE OF MESH POSITION
		uiPos2 = ofMap(handPosData.z, -1, 1, -1000, 1000);
		//uiPos5 = lefthand.z * -500;
	}
	if (handPosData.w != 0.0) {
		
		checkerRY(handPosData.w);
		//RIGHT HAND Y CONTROLS NUMBER OF POINTS
		uiAmount = ofMap(handPosData.w, -1, 1, 0, 50);
		
	}

	ofSeedRandom(30);
	mesh.clear();

	//CREATE RANDOM VEC3 VALUES FOR EACH POINT AND ANIMATE, CONTSTAINED WITHIN POSITIONS SET BY 'UIPOS' VALUES 
	for (int i = 0; i < uiAmount; i++) {
		ofVec3f position = ofVec3f(
			ofMap(ofNoise(ofRandom(1000), ofGetElapsedTimef() * 0.06), 0, 1, uiPos1, uiPos2),
			ofMap(ofNoise(ofRandom(1000), ofGetElapsedTimef() * 0.06), 0, 1, uiPos3, uiPos4),
			ofMap(ofNoise(ofRandom(1000), ofGetElapsedTimef() * 0.06), 0, 1, uiPos5, uiPos6)
            );
		mesh.addVertex(position);
		mesh.addColor(ofColor(255,255,255,100));
	}


	//DRAW VERTEX LINE ONLY IF WITHIN DISTANCE SET BY 'UIDISTANCE'
	for (int i = 0; i < mesh.getVertices().size(); i++) {

		auto position = mesh.getVertex(i);

		for (int j = 0; j < mesh.getVertices().size(); j++) {
		
			auto distance = glm::distance(position, mesh.getVertex(j));

			if (distance < uiDistance) {
				mesh.addIndex(i);
				mesh.addIndex(j);
			}
		}
	}

	//GET/SET RMS REACTIVITY VALUES

	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back(scaledVol);

	//if we are bigger the the size we want to record - lets drop the oldest value
	if (volHistory.size() >= 400) {
		volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	ofColor color1(0, 0, 255);
	ofColor color2(scaledVol *375, scaledVol*375, 255);
	ofBackgroundGradient(color1, color2, OF_GRADIENT_BAR);
	

	ofSetLineWidth(0.5);


	//CENTER DRAW POINT
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	//DRAW CONSTALATION
	if (uiPoints == 1) {
		for (int i = 0; i < mesh.getVertices().size(); i++) {


			rotate++;

			ofIcoSpherePrimitive ico;
			ico.set(scaledVol * 100, 1);

			//getPosition();

			ofSetColor(255);

			ico.setPosition(mesh.getVertex(i));
			ico.rotate(rotate /5, 0.3, 0.3, 0.3);

			ico.drawWireframe();
		}
	}

	//DRAW WIRE FRAME OF USER BODY
	kinectscene.draw();

	//DRAW CONSTALTION
	ofSetLineWidth(2.0);
		mesh.draw();


		

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		fs = !fs;
		ofSetFullscreen(fs);
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



//--------------------------------------------------------------
void ofApp::newUser(NCGenericEventArg& arg) {
	cout << arg.userid << endl;
}

//--------------------------------------------------------------
void ofApp::lostUser(NCGenericEventArg& arg) {
	cout << arg.userid << endl;
}
