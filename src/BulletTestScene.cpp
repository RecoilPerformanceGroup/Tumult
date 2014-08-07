//
//  BulletTestScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "BulletTestScene.h"

void BulletTestScene::setup() {
    name = "Spring Scene";
    oscAddress = "/BulletTestScene";
    
    light.setPointLight();
    light.setPosition(-2, -2, -2);
    
    enabled = true;
    
    
    world.setup();
    //world.setCamera(camRef);
	
	/*ground = new ofxBulletBox();
	ground->create( world.world, ofVec3f(0., 1.0, 0.), 0., 50., 1.f, 50.f );
	ground->setProperties(.25, .95);
	ground->add();
	*/
	
    world.setGravity(ofVec3f(-0.02,0,0));
    
	jointLength = .005f;
	
	shapes.push_back( new ofxBulletSphere() );
	((ofxBulletSphere*)shapes[0])->create( world.world, ofVec3f(0, 0.0, 0.0), 0.2f, 0.02f );
	shapes[0]->add();
	
	
	joints.push_back( new ofxBulletJoint() );
	joints[joints.size()-1]->create( world.world, shapes[0], ofVec3f(0, 0.0, 0.0));
	joints[joints.size()-1]->add();
	
	bDrawDebug	= false;
	bSpacebar	= false;
	bShapesNeedErase = false;

}
void BulletTestScene::draw(int _surfaceId) {
    
    //glEnable( GL_DEPTH_TEST );
	//camera.begin();
	
	ofSetLineWidth(2.f);
	if(bDrawDebug) world.drawDebug();
	
	ofSetColor(255, 255, 255);
	//ofDrawSphere(mousePos, .15f);
	
	//ofEnableLighting();
	light.enable();
	//light.setPosition( mousePos );
	
	ofSetColor(100, 100, 100);
	//ground->draw();
	
	ofSetColor(255, 0, 255);
	for(int i = 0; i < shapes.size(); i++) {
		ofSetColor(255);
		shapes[i]->draw();
	}
	ofSetColor(220, 220, 220);
	for(int i = 0; i < joints.size(); i++) {
		joints[i]->draw();
	}
    
	light.disable();
	//ofDisableLighting();
	
	//camera.end();
	//glDisable(GL_DEPTH_TEST);
	
	ofSetColor(255, 255, 255);
	/*stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(),0) << endl;
	ss << "num shapes: " << (shapes.size()) << endl;
	ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
	ss << "break joints with spacebar: " << bSpacebar << endl;
	ofDrawBitmapString(ss.str().c_str(), 10, 10);*/
    
    
}

void BulletTestScene::update() {
    
    world.update();
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
	//mousePos = camRef->screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
    
	joints[0]->updatePivotPos( centerPos, 0.01f );
	
	if(bSpacebar) {
		for (int i = 1; i < joints.size(); i++) {
			delete joints[i];
		}
		joints.erase(joints.begin()+1, joints.end());
		bSpacebar = false;
		bShapesNeedErase = true;
	}
	
	for (int i = 1; i < joints.size(); i++) {
		if(i == 1) {
			//joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength*0.2f );
		} else {
			//joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength );
		}
        
	}

}


void BulletTestScene::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    gui->addSlider("posx", -2, 2, &centerPos.x);
    gui->addSlider("posy", -2, 2, &centerPos.y);
    gui->addSlider("posz", -2, 2, &centerPos.z);
}

void BulletTestScene::receiveOsc(ofxOscMessage * m, string rest) {
}

void BulletTestScene::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}


//--------------------------------------------------------------
void BulletTestScene::keyPressed(int key){
    
    
    switch (key) {
		case ' ': {
			bSpacebar = true;
			break;
        }
		case 'd': {
			bDrawDebug = !bDrawDebug;
			break;
        }
        case 'a': {
            
            float rsize = ofRandom(.01, .05);
            shapes.push_back( new ofxBulletSphere() );
            
            ofVec3f pos = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.1, 0.1));
            
            float mass = 0.10;
            
            ((ofxBulletSphere*) shapes[shapes.size()-1])->create( world.world, pos, mass, rsize );
            shapes[shapes.size()-1]->add();
            
            shapeColors.push_back( colors[0] );
            
            joints.push_back( new ofxBulletJoint() );
            
            int rshape = ofRandom(shapes.size()-1);
            while(rshape == shapes.size()-1) {
                rshape = ofRandom(shapes.size()-1);
            }
            
            joints[joints.size()-1]->create(world.world, shapes[shapes.size()-1], shapes[0]);
            joints[joints.size()-1]->add();
            
			break;
        }
        case 'j': {
            
            
            joints.push_back( new ofxBulletJoint() );
            
            int rshape = ofRandom(shapes.size()-1);
            int rshapeTwo = rshape;
            while( rshapeTwo == rshape) {
                rshapeTwo = ofRandom(shapes.size()-1);
            }
            
            joints[joints.size()-1]->create(world.world, shapes[rshapeTwo], shapes[rshape]);
            joints[joints.size()-1]->add();
            
            
            break;
            
        }
            
            
            
		//default:
		//	break;
	}
    
}

//--------------------------------------------------------------
void BulletTestScene::keyReleased  (int key){

}

//--------------------------------------------------------------
void BulletTestScene::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void BulletTestScene::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BulletTestScene::mouseReleased(int x, int y, int button){
}

