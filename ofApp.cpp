#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {

	delete this->boundsShape;

	for (ofxBulletBox* tmp : this->boxes) { 
		delete tmp;
	}
	this->boxes.clear();
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->cam.setPosition(ofVec3f(0, 3.f, -30.f));
	this->cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	this->world.setup();
	this->world.enableGrabbing();
	this->world.setCamera(&this->cam);
	this->world.setGravity(ofVec3f(0.0, 64.0, 0));

	ofVec3f start_location;
	ofPoint dimens;
	float bounds_width = 400;
	float h_width = bounds_width * 0.5;
	float depth = 10.;
	float h_depth = depth * 0.5;
	this->boundsShape = new ofxBulletCustomShape();
	this->boundsShape->create(world.world, ofVec3f(0, 0, 0), 10.);

	for (int i = 0; i < 6; i++) {
		if (i == 0) { // ground //
			start_location.set(0., h_width + h_depth, 0.);
			dimens.set(bounds_width, depth, bounds_width);
		}
		else if (i == 1) { // back wall //
			start_location.set(0, 0, h_width + h_depth);
			dimens.set(bounds_width, bounds_width, depth);
		}
		else if (i == 2) { // right wall //
			start_location.set(h_width + h_depth, 0, 0.);
			dimens.set(depth, bounds_width, bounds_width);
		}
		else if (i == 3) { // left wall //
			start_location.set(-h_width - h_depth, 0, 0.);
			dimens.set(depth, bounds_width, bounds_width);
		}
		else if (i == 4) { // ceiling //
			start_location.set(0, -h_width - h_depth, 0.);
			dimens.set(bounds_width, depth, bounds_width);
		}
		else if (i == 5) { // front wall //
			start_location.set(0, 0, -h_width - h_depth);
			dimens.set(bounds_width, bounds_width, depth);
		}
		btBoxShape* boxShape = ofBtGetBoxCollisionShape(dimens.x, dimens.y, dimens.z);
		this->boundsShape->addShape(boxShape, start_location);
	}
	this->boundsShape->add();

	for (int i = 0; i < 512; i++) {
		ofxBulletBox* box = new ofxBulletBox();
		float size = 10;
		box->create(this->world.world, ofVec3f(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100)), 1.0, size, size, size);
		box->setRestitution(1.0);
		box->add();

		this->boxes.push_back(box);

		ofColor color;
		color.setHsb(ofRandom(255), 255, 255);
		this->boxes_color.push_back(color);
	}

	this->light.setPosition(ofVec3f(0, 0, 512));
}

//--------------------------------------------------------------
void ofApp::update() {
	if (!ofGetMousePressed()) {
		ofVec3f diff = ofVec3f(0, 0, 0) - boundsShape->getPosition();
		diff *= 500.f;
		boundsShape->applyCentralForce(diff);
	}

	this->world.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	//ofEnableLighting();
	//this->light.enable();

	ofNoFill();
	if (ofGetMousePressed()) {
		ofSetColor(255, 255, 0);
	} else {
		ofSetColor(255);
	}
	boundsShape->transformGL();
	ofDrawBox(ofVec3f(0, 0, 0), 400);
	boundsShape->restoreTransformGL();
	ofFill();

	for (int i = 0; i < this->boxes.size(); i++) {
		ofSetColor(this->boxes_color[i], 128);
		ofDrawBox(this->boxes[i]->getPosition(), 3, 3, 3);

		for (int j = i + 1; j < this->boxes.size(); j++) {
			float distance = this->boxes[i]->getPosition().distanceSquared(this->boxes[j]->getPosition());
			if (distance < 40 * 40) {		
				ofSetColor(this->boxes_color[i], ofMap(distance, 0, 40 * 40, 255, 0));
				ofDrawLine(this->boxes[i]->getPosition(), this->boxes[j]->getPosition());
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'a') {
		this->world.setGravity(ofVec3f(0.0, 0.0, 0));
	}

	if (key == 's') {
		this->world.setGravity(ofVec3f(0.0, 64.0, 0));
	}
}

//========================================================================
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
