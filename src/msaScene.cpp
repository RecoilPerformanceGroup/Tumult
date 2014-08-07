//
//  ClothScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "ClothScene.h"
#include "MSAPhysics3D.h"

#define	SPRING_MIN_STRENGTH		0.05
#define SPRING_MAX_STRENGTH		0.1

#define	SPRING_MIN_WIDTH		0.02
#define SPRING_MAX_WIDTH		0.05

#define NODE_MIN_RADIUS			0.15
#define NODE_MAX_RADIUS			0.55

#define MIN_MASS				1
#define MAX_MASS				3

#define MIN_BOUNCE				0.02
#define MAX_BOUNCE				0.09

#define	FIX_PROBABILITY			10		// % probability of a particle being fixed on creation

#define FORCE_AMOUNT			0.10

#define EDGE_DRAG				0.98

#define	GRAVITY					1

#define MAX_ATTRACTION			0.010
#define MIN_ATTRACTION			0.03

#define SECTOR_COUNT			1		// currently there is a bug at sector borders

bool				mouseAttract	= false;
bool				doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
bool				doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
bool				doRender		= true;
int					forceTimer		= false;


float				rotSpeed		= 0;
float				mouseMass		= 1;

static int			width;
static int			height;


msa::physics::World3D		physics;
msa::physics::Particle3D		mouseNode;


void initScene() {
	// clear all particles and springs etc
	physics.clear();
	
	// you can add your own particles to the physics system
	physics.addParticle(&mouseNode);
	mouseNode.makeFixed();
	mouseNode.setMass(MIN_MASS);
	mouseNode.moveTo(ofVec3f(0, 0, 0));
	mouseNode.setRadius(NODE_MAX_RADIUS);
	
	// or tell the system to create and add particles
	physics.makeParticle(ofVec3f(-width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top left back and fix
	physics.makeParticle(ofVec3f( width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
	physics.makeParticle(ofVec3f(-width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
	physics.makeParticle(ofVec3f( width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix
}

void ClothScene::setup() {
    name = "Spring Scene";
    oscAddress = "/ClothScene";
    
    light.setPointLight();
    light.setPosition(-2, -2, -2);
    
    enabled = true;
    
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
		
	width = 2.0;
	height = 2.0;
	
	//	physics.verbose = true;			// dump activity to log
	physics.setGravity(ofVec3f(0, GRAVITY, 0));
	
	// set world dimensions, not essential, but speeds up collision
	physics.setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
	physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
	physics.setDrag(1);		// FIXTHIS
	physics.enableCollision();
	
	initScene();
	
	// setup lighting
	//GLfloat mat_shininess[] = { 50.0 };
	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_position[] = { 0, height/2, 0.0, 0.0 };
	//glShadeModel(GL_SMOOTH);
	
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT0);
	
	// enable back-face culling (so we can see through the walls)
	glCullFace(GL_BACK);
    

}

void ClothScene::draw(int _surfaceId) {
    
    if(doRender) {
        //glEnable(GL_CULL_FACE);
				
		//ofTranslate(width/2, 0, -width / 3);		// center scene
		static float rot = 0;
		ofRotate(rot, 0, 1, 0);			// rotate scene
		rot += rotSpeed;						// slowly increase rotation (to get a good 3D view)
		
		if(forceTimer) {
			float translateMax = forceTimer;
			ofTranslate(ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax));
			forceTimer--;
		}
		
/*
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		// draw right wall
		glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, width/2);
		glColor3f(1, 1, 1);				glVertex3f(width/2, -height, width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, -width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, -width/2);
		
		// back wall
		glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, -width/2);
		glColor3f(1, 1, 1);				glVertex3f(width/2, -height, -width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, -width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, -width/2);
		
		// left wall
		glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, -width/2);
		glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, -width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, width/2);
		
		// front wall
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, width/2);
		glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, width/2);
		glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, width/2);
		
		// floor
		glColor3f(.8, 0.8, 0.8);
		glVertex3f(width/2, height+1, width/2);
		glVertex3f(width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, width/2);
		glEnd();
		
        //		glEnable(GL_LIGHTING);*/
		
		// draw springs
		ofSetColor(150, 150, 150);
        
		for(int i=0; i<physics.numberOfSprings(); i++) {
			msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physics.getSpring(i);
			msa::physics::Particle3D *a = spring->getOneEnd();
			msa::physics::Particle3D *b = spring->getTheOtherEnd();
			
            ofVec3f vec = b->getPosition() - a->getPosition();
			float dist = vec.length();
			float angle = acos( vec.z / dist ) * RAD_TO_DEG;
			if(vec.z <= 0 ) angle = -angle;
			float rx = -vec.y * vec.z;
			float ry =  vec.x * vec.z;
			
			ofPushMatrix();
			ofTranslate(a->getPosition().x, a->getPosition().y, a->getPosition().z);
			ofRotate(angle, rx, ry, 0.0);
			float size  = ofMap(spring->getStrength(), SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH, SPRING_MIN_WIDTH, SPRING_MAX_WIDTH);
			
			ofScale(size, size, dist);
			ofTranslate(0, 0, 0.5);
            ofDrawBox(0.02);
			ofPopMatrix();
		}
		
		
		// draw particles
		//glAlphaFunc(GL_GREATER, 0.5);
		
		ofEnableNormalizedTexCoords();
		//ballImage.getTextureReference().bind();
		for(int i=0; i<physics.numberOfParticles(); i++) {
			msa::physics::Particle3D *p = physics.getParticle(i);
			
            if(p->isFixed()) ofSetColor(0,0,0);
			else ofSetColor(255,255,255);
            
			glEnable(GL_ALPHA_TEST);
			
			// draw ball
			ofPushMatrix();
			ofTranslate(p->getPosition().x, p->getPosition().y, p->getPosition().z);
			ofRotate(180-rot, 0, 1, 0);
            
			//glBegin(GL_QUADS);
			/*glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
			glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());*/
            
			//glEnd();
            
            ofDrawSphere(0, 0, 0.05);
            
			ofPopMatrix();
			
			glDisable(GL_ALPHA_TEST);
			
			float alpha = ofMap(p->getPosition().y, -height * 1.5, height, 0, 1);
			if(alpha>0) {
				ofPushMatrix();
				ofTranslate(p->getPosition().x, height, p->getPosition().z);
				ofRotate(-90, 1, 0, 0);
				//glColor4f(0, 0, 0, alpha * alpha * alpha * alpha);
                //				ofCircle(0, 0, p->getRadius());
				float r = p->getRadius() * alpha;
				
                /*glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(-r, -r);
				glTexCoord2f(1, 0); glVertex2f(r, -r);
				glTexCoord2f(1, 1); glVertex2f(r, r);
				glTexCoord2f(0, 1); glVertex2f(-r, r);
				glEnd();*/
				ofDrawSphere(0, 0, 0.05);
                
                ofPopMatrix();
			}
			
		}
		//ballImage.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();
		
	}
    
    //glDisable(GL_CULL_FACE);
	
    
}

void ClothScene::update() {
    
    width = 2;
	height = 2;
	
	physics.update();
    
}


void ClothScene::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
}

void ClothScene::receiveOsc(ofxOscMessage * m, string rest) {
}

void ClothScene::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}

void addRandomParticle() {
	float posX		= ofRandom(-width/2, width/2);
	float posY		= ofRandom(0, height);
	float posZ		= ofRandom(-width/2, width/2);
	float mass		= ofRandom(MIN_MASS, MAX_MASS);
	float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
	float radius	= ofMap(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS, NODE_MAX_RADIUS);
	
	// physics.makeParticle returns a particle pointer so you can customize it
	msa::physics::Particle3D *p = physics.makeParticle(ofVec3f(posX, posY, posZ));
	
	// and set a bunch of properties (you don't have to set all of them, there are defaults)
	p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
	
	// add an attraction to the mouseNode
	if(mouseAttract) physics.makeAttraction(&mouseNode, p, ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
}

void addRandomSpring() {
	msa::physics::Particle3D *a = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
	msa::physics::Particle3D *b = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
	physics.makeSpring(a, b, ofRandom(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), ofRandom(10, width/2));
}


void killRandomParticle() {
	msa::physics::Particle3D *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
	if(p && p != &mouseNode) p->kill();
}

void killRandomSpring() {
	msa::physics::Spring3D *s = physics.getSpring( floor(ofRandom(0, physics.numberOfSprings())));
	if(s) s->kill();
}

void killRandomConstraint() {
	msa::physics::Constraint3D *c = physics.getConstraint(floor(ofRandom(0, physics.numberOfConstraints())));
	if(c) c->kill();
}


void toggleMouseAttract() {
	mouseAttract = !mouseAttract;
	if(mouseAttract) {
		// loop through all particles and add attraction to mouse
		// (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
		for(int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(&mouseNode, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
	} else {
		// loop through all existing attractsions and delete them
		for(int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
	}
}

void addRandomForce(float f) {
	forceTimer = f;
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		if(p->isFree()) p->addVelocity(ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
	}
}

void lockRandomParticles() {
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		if(ofRandom(0, 100) < FIX_PROBABILITY) p->makeFixed();
		else p->makeFree();
	}
	mouseNode.makeFixed();
}

void unlockRandomParticles() {
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		p->makeFree();
	}
	mouseNode.makeFixed();
}


//--------------------------------------------------------------
void ClothScene::keyPressed(int key){
	switch(key) {
            
		case 'a': toggleMouseAttract(); break;
		case 'p': for(int i=0; i<100; i++) addRandomParticle(); break;
		case 'P': for(int i=0; i<100; i++) killRandomParticle(); break;
		case 's': addRandomSpring(); break;
		case 'S': killRandomSpring(); break;
		case 'c': physics.isCollisionEnabled() ? physics.disableCollision() : physics.enableCollision(); break;
		case 'C': killRandomConstraint(); break;
		case 'r': doRender ^= true; break;
		case 'f': addRandomForce(FORCE_AMOUNT); break;
		case 'F': addRandomForce(FORCE_AMOUNT * 3); break;
		case 'l': lockRandomParticles(); break;
		case 'u': unlockRandomParticles(); break;
		case ' ': initScene(); break;
		case 'x': doMouseXY = true; break;
		case 'z': doMouseYZ = true; break;
		case ']': rotSpeed += 0.01f; break;
		case '[': rotSpeed -= 0.01f; break;
		case '+': mouseNode.setMass(mouseNode.getMass() +0.1); break;
		case '-': mouseNode.setMass(mouseNode.getMass() -0.1); break;
		case 'm': mouseNode.hasCollision() ? mouseNode.disableCollision() : mouseNode.enableCollision();
            
	}
}

//--------------------------------------------------------------
void ClothScene::keyReleased  (int key){
	switch(key) {
		case 'x': doMouseXY = false; break;
		case 'z': doMouseYZ = false; break;
	}
}

//--------------------------------------------------------------
void ClothScene::mouseMoved(int x, int y ) {
	static int oldMouseX = -10000;
	static int oldMouseY = -10000;
	int velX = x - oldMouseX;
	int velY = y - oldMouseY;
	if(doMouseXY) mouseNode.moveBy(ofVec3f(velX, velY, 0));
	if(doMouseYZ) mouseNode.moveBy(ofVec3f(velX, 0, velY));
	oldMouseX = x;
	oldMouseY = y;
}

//--------------------------------------------------------------
void ClothScene::mouseDragged(int x, int y, int button){
	switch(button) {
		case 0:	doMouseXY = true; mouseMoved(x, y); break;
		case 2: doMouseYZ = true; mouseMoved(x, y); break;
	}
}

//--------------------------------------------------------------
void ClothScene::mouseReleased(int x, int y, int button){
	doMouseXY = doMouseYZ = false;
}

