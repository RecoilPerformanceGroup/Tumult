//
//  TestScene.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#pragma once
#include "ContentScene.h"
#include "ofxBullet.h"
#include "ofMain.h"

#include "ofxGaussianBlur.h"
#include "ofxBlur.h"

class BulletTestScene : public ContentScene {
    
public:
    void draw(int _surfaceId);
    void beginWorld(int _surfaceId);
    void endWorld(int _surfaceId);
    void update();
    void setup();
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    void receiveOsc(ofxOscMessage * m, string rest);
    
    ofLight light;
    ofLight dirLight;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    ofVec3f						centerPos;
    
    
    ofxBulletWorldRigid			world;
	ofxBulletBox*				ground;
	
	vector<ofxBulletBaseShape*> shapes;
	vector<ofxBulletJoint*>		joints;
	vector<ofColor>				shapeColors;
	ofColor						colors[4];
	
	bool						bDrawDebug;
	bool						bSpacebar;
	bool						bShapesNeedErase;
	
	float						jointLength;
    
    ofxBlur gaus;
    
    ofTexture texture;
    
    ofVbo vbo;
    ofShader shader;
    
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    float camDist;

    
};
