#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxXmlSettings.h"
#include "ofxOscReceiver.h"
#include "StereoPlane.h"
#include "ContentScene.h"
#include "TestScene.h"
#include "tumultVideoMixer.h"

//#include "BulletTestScene.h"
#include "ofxUI.h"


class ofApp : public ofBaseApp
{
public:
	
	void setup();
	void update();
	void draw();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void exit();
    ofxSyphonServer sbsOutputServer;
    bool showGrid;
    
    ofxOscReceiver oscReceiver;
    // ofxOscSender oscSender;
    
    float eyeSeperation;
    
    bool drawChessboards = false;
    bool drawGrids = false;
    float aspect = 1.0;
    
    float guiWidth;
    
    bool drawTimeline;
    bool drawGUI;
    bool drawMonitor;
    
    vector<ofxUIScrollableCanvas *> guis;
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofFbo fbo;
    StereoPlane * wall;
    
    // --
    StereoPlane * activePlane;
    int activePlaneIndex;
    
    void drawFloor();
    vector<StereoPlane *> planes;
    ofxXmlSettings settings;
    
    void drawScenes(int _surfaceId=0);
    
    //BulletTestScene * clothScene;
    VideoMixer * vidMix;
    vector<ContentScene*> contentScenes;
    
};