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

    light.setAmbientColor(ofColor(255,255,255,255));
    light.setDiffuseColor(ofColor(255,100,100,255));
    light.setPosition(-1, -1, -1);
    light.setAttenuation(5);

    light.setPosition(-1, -1, 2);
    
    dirLight.setDirectional();
    dirLight.setPosition(1, 1, -1);
    dirLight.lookAt(ofVec3f(0,0,0));
    
    enabled = true;

    world.setup();
    //world.setCamera(camRef);
	
	/*ground = new ofxBulletBox();
	ground->create( world.world, ofVec3f(0., 1.0, 0.), 0., 50., 1.f, 50.f );
	ground->setProperties(.25, .95);
	ground->add();
	*/
	
    world.setGravity(ofVec3f(-0.0,0.00,0.0));
    
	jointLength = .005f;
	
	shapes.push_back( new ofxBulletSphere() );
	((ofxBulletSphere*)shapes[0])->create( world.world, ofVec3f(0, 0.0, 0.0), 1.2f, 0.02f );
	shapes[0]->add();
	
	
	joints.push_back( new ofxBulletJoint() );
	joints[joints.size()-1]->create( world.world, shapes[0], ofVec3f(0, 0.0, 0.0));
	joints[joints.size()-1]->add();
	
	bDrawDebug	= false;
	bSpacebar	= false;
	bShapesNeedErase = false;
    
    ofDisableArbTex();
    ofLoadImage(texture, "dot.png");
    
    //gaus.setup(1920, 1080);
    //gaus.allocate(1920, 1080);
    //gaus.setRadius(0.1);
    
    points.push_back(ofVec3f(0.0, 0.0, 0.0));
    // we are passing the size in as a normal x position
    float size = 0.02f;
    sizes.push_back(ofVec3f(size));

    
    points.push_back(ofVec3f(0.0, 0.0, 0.0));
    // we are passing the size in as a normal x position
    size = 0.02f;
    sizes.push_back(ofVec3f(size));
    
    // upload the data to the vbo
	int total = (int)points.size();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    
    

}

void BulletTestScene::beginWorld(int _surfaceId) {
}

void BulletTestScene::endWorld(int _surfaceId) {
}

void BulletTestScene::draw(int _surfaceId) {
    
    ofEnableLighting();
    
    ofSetLineWidth(2.f);
    //glEnable( GL_DEPTH_TEST );
	//camera.begin();
	
	if(bDrawDebug) world.drawDebug();
	
	ofSetColor(255, 255, 255, 255);
	light.enable();
	
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
    
    glDepthMask(GL_FALSE);
    texture.bind();

    //vbo.draw(GL_POINTS, 0, (int)points.size());
    //glBegin(GL_POINTS);
    
    for(int i = 0; i < shapes.size(); i++) {
        
        //glVertex3f(shapes[i]->getPosition().x, shapes[i]->getPosition().y, shapes[i]->getPosition().z);
        
        ofPushMatrix();
        ofTranslate(shapes[i]->getPosition());
        ofDrawPlane(0, 0, 0.1, 0.1);
        ofPopMatrix();
        
	}
    
    
    //glEnd();

    texture.unbind();
    
    glDepthMask(GL_TRUE);
    
    ofDisablePointSprites();
	ofDisableBlendMode();
    
    ofEnableAlphaBlending();
	ofSetColor(255, 0, 255);
	for(int i = 0; i < shapes.size(); i++) {
		ofSetColor(255,255,255,255);
		//shapes[i]->draw();
        //ofDrawBox(shapes[i]->getPosition().x, shapes[i]->getPosition().y, shapes[i]->getPosition().z, 0.1, 0.1, 0.1);
        
	}
	ofSetColor(255, 255, 255, 255);
	for(int i = 0; i < joints.size(); i++) {
		//joints[i]->draw();
	}
    
    
	light.disable();

    
}

void BulletTestScene::update() {
    
    
    for(int i = 0; i < shapes.size(); i++) {
        
        points[i] = shapes[i]->getPosition();
        
	}
    
    
    world.update();
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
	//mousePos = camRef->screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
    
	joints[0]->updatePivotPos( centerPos, 0.01f );
	
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
    gui->addSlider("posz", -4, 4, &centerPos.z);
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
            
            float mass = 4.10;
            
            ((ofxBulletSphere*) shapes[shapes.size()-1])->create( world.world, pos, mass, rsize );
            shapes[shapes.size()-1]->add();
            
            
            points.push_back(pos);
            
            // we are passing the size in as a normal x position
            float size = rsize;
            sizes.push_back(ofVec3f(size));
            
            shapeColors.push_back( colors[0] );
            
            joints.push_back( new ofxBulletJoint() );
            
            int rshape = ofRandom(shapes.size()-1);
            while(rshape == shapes.size()-1) {
                rshape = ofRandom(shapes.size()-1);
            }
            
            vector<ofVec3f> positions;
            
            positions.push_back(ofVec3f(1,1,0));
            positions.push_back(ofVec3f(1,-1,0));
            positions.push_back(ofVec3f(-1,1,0));
            positions.push_back(ofVec3f(-1,-1,0));
            
            joints[joints.size()-1]->create(world.world, shapes[shapes.size()-1], positions[ofRandom(positions.size())]);
            
            
            joints[joints.size()-1]->add();
            
            
            joints.push_back( new ofxBulletJoint() );
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

