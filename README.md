CODING 2: FINAL PROJECT READ ME

Note to the marker::  My code to be marked is the ofApp.h, ofApp.cpp and also in the ncKinectv2Core.cpp (found in add ons -> ofxNCKinect-master -> src -> core ->), specifically the ofVec4f ncKinectv2Core::bothHands() function.


Creative Motivations::

The aim of this project was to create a piece that works both as a performance tool for myself and also a public installation. A borrowed Kinect V2 gave me the idea to create an audio visual performance where motion is directly tracked to sound. I enjoyed the concept of being able to conduct my synths as though they were an orchestra.  I was also inspired by a number of artists who had used skeletal tracking in their work. A few examples of specific inspiration are below:

Joao Martinho Moura: http://jmartinho.net/works/

Saeko Ehara: https://www.instagram.com/p/CG2E-l_gANk/

Dan Molnar: https://www.instagram.com/function.str/

Design and Implementation::

This project utilises modified ofxNCkinect (https://github.com/wearenocomputer/ofxNCKinect) and ofxOSC openframeworks addons to receive left and right hand position to manipulate projected audio reactive visual, and control sound in Ableton 11. 

When beginning this project I assumed getting the XY data from the left and right hand joints detected by the kinect would be simple. I was very wrong. 

I had to create a function based on ncKinectv2Core::getBodyData() that would give me specific joint values, in this case left and right hands. I tried to have a function for each hand returning XYZ positions as a vec3. However they did not work simultaneously and I was unable to fix this. So I resorted to a single function returning a vec4 of both hands XY values. Another issue was the kinect would randomly return zero values. After many unsuccessful attempts to solve this issue I resorted to simply excluding them with an if (x != 0) statement for each variable. 

The primary constellation visual is simply a mesh positioned with a vec3. Within the positions set by the vec3 randomised, animated vertices are drawn. The x-coord of the user's hands control the width of the area in which points are drawn, the y-coords are fixed. The right hand y axis controls the number of points drawn in the mesh up to 50. The left hand y axis determines the distance between two vertices at which a line connection is drawn.

At each vertex a icosphere is drawn. Its rotation is animated and then scaled based upon the scaledVol variable which is created by audio analysis of the output from Ableton using RMS. In future I plan to change this to FFT for better control. 

To control the audio XY position data from the hands is output via ofxOSC. Each of the 4 checker() functions takes in the XY coords (separately) of each hand that have had the zero values filtered out. Each function then sets an address, scales the variable as a float within 0 to 1 and sends the OSC message to Ableton. The Ableton uses the Live Grabber (https://www.showsync.com/tools#livegrabber) Max4Live devices to receive and control the parameters within.
I am currently using a MIDI controller on the floor to change between scenes in Ableton.


Next steps::

-Solve major technical issues, find the cause of random zero values and fix. Also work out how to track multiple points at once as vec3s

-Implement FFT for audio reactivity. Or perhaps even perform audio analysis on individual channels within Ableton with max4live devices, then send to visual studio via OSC. This would allow much more advanced reactivity

-Experiment with other visuals and even use the live video feed from the kinect to incorporate into visual performance

-Find a less intrusive way to change scenes in ableton. Either using the kinect or via a wrist mounted OSC device?

-Incorporate control of analogue modular gear via the kinect. Interesting clash of analogue and obsolete digital.

