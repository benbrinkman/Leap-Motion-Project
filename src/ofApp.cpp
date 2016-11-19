#include "ofApp.h"
//#include "ink.h"

//--------------------------------------------------------------
void ofApp::setup(){

	for (int i = 0; i < 10; ++i) {
		inkBlot[i].angle = 0;
		inkBlot[i].decay = 0.01;
		inkBlot[i].diamater = 1;
		inkBlot[i].pos = ofVec2f(0, 0);
		inkBlot[i].speed = 2;
		inkBlot[i].opacity = 1;
	}
	switchState = false;
	counter = 0;
	m_title = true;
	m_font.load("verdana.ttf", 16, true, true);
	m_inkCollision = 0;
	m_colDiameter = 30;
	m_keyTap = false;
	m_inkFired = false;
	m_inkInit = false;
	//we are initializing and attatching our onLeapFrame function to the m_device to be called when updating
	m_device.connectEventHandler(&ofApp::onLeapFrame, this);
	m_palmPos = ofVec3f(0, 0, 500);
	m_palmRotY = 0.0f;
	m_Random.x = ofRandom(150, ProjectConstants::WINDOW_SIZE_X -150);
	
	m_Random.y = ofRandom(100, ProjectConstants::WINDOW_SIZE_Y);

	m_health = 10;
	
	for (int i = 0; i < 10; i++) {
		enemy[i].pos.x = ofRandom(0, ProjectConstants::WINDOW_SIZE_X);
		enemy[i].pos.y = ofRandom(-10, -500);
		enemy[i].speed = ofRandom(1, 4);
		enemy[i].active = true;
		enemy[i].fishAnimState = 1;

		enemy[i].fish.load("fish1.png");
	}
	m_dead = false;
	m_shipImg.load("squid.png");
	m_sideImg.load("side.png");
	m_background.load("bg.jpg");
	m_titleScreen.load("titleScreen.png");
	m_gameover.load("gameover.jpg");
	m_points = 0;
	Leap::Controller * controller = m_device.getController();
	controller->enableGesture(Leap::Gesture::TYPE_KEY_TAP, true);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!m_title) {
		if (!m_dead) {
			if (m_health < 0) {
				m_dead = true;
			}

			m_device.update();
			counter++;
			if (counter > 10) {
				counter = 0;
				if (switchState == false)
					switchState = true;
				else
					switchState = false;
			}
			/*********************ENEMY BEHAVIOUR************************/
			for (int i = 0; i < 10; i++) {
				enemy[i].pos.y += enemy[i].speed;
				if (enemy[i].pos.y > ProjectConstants::WINDOW_SIZE_Y) {
					enemy[i].pos.x = ofRandom(0, ProjectConstants::WINDOW_SIZE_X);
					enemy[i].pos.y = -10;
					enemy[i].fishAnimState = 1;
					enemy[i].active = true;
				}
				if (fabs(m_object.x - enemy[i].pos.x) < 50 && fabs(m_object.y - enemy[i].pos.y) < 50 && enemy[i].active) {
					enemy[i].pos.x = ofRandom(0, ProjectConstants::WINDOW_SIZE_X);
					enemy[i].pos.y = -10;
					enemy[i].fishAnimState = 1;
					m_health--;
				}

				if (switchState && enemy[i].fishAnimState != 3)
					enemy[i].fishAnimState = 2;
				else if (!switchState && enemy[i].fishAnimState != 3)
					enemy[i].fishAnimState = 1;
			}
			

			/**********************LEAP GESTURES******************************/

			const Leap::HandList& hands = m_frame.hands();
			const Leap::GestureList& gestures = m_frame.gestures();
			for (int i = 0; i < gestures.count(); i++) {

				const Leap::Gesture& gesture = gestures[i];
				Leap::Gesture::Type type = gesture.type();

				if (type == Leap::Gesture::Type::TYPE_KEY_TAP) {
					ofLogNotice("KEY");
					m_keyTap = true;
					if (!m_inkFired) {
						m_inkFired = true;
						m_inkInit = true;
					}
				}
			}


			/**********************LEAP DETECT HANDS******************************/
			for (int i = 0; i < hands.count(); i++) {
				const Leap::Hand& hand = hands[i];								//get the hand
				const Leap::Vector palmPos = hand.palmPosition();					//get the palm pos in Leap's Vector
				m_palmPos = ofxLeapMotion::toVec3(palmPos);	//convert to openframeworks vector - ofVec3f
				m_palmRotY = ofRadToDeg(hand.direction().yaw());
			}
			//ofLogNotice("Rot: " + ofToString(m_palmRotY));



			/******************ALTERATE CONTROLS FOR NO LEAP*********************/
			//m_object.x = ofGetMouseX();
			//m_object.y = ofGetMouseY();

			


			/*************************PLAYER CONTROL******************************/

			m_object.x = (ProjectConstants::WINDOW_SIZE_X / 2) + m_palmPos.x * 3.5;
			m_object.y = m_palmPos.z * 4;
			m_object.z = m_palmPos.z;

			m_object.x = ofClamp(m_object.x, 0, ProjectConstants::WINDOW_SIZE_X);
			m_object.z = ofClamp(m_object.z, 0, ProjectConstants::WINDOW_SIZE_Y);


			/**************************OBJECT CONTROL*****************************/

			if (fabs(m_object.x - m_Random.x) < 50 && fabs(m_object.y - m_Random.y) < 50) {
				m_Random.x = ofRandom(150, ProjectConstants::WINDOW_SIZE_X -150);
				m_Random.y = ofRandom(100, ProjectConstants::WINDOW_SIZE_Y);
				m_points++;
				ofResetElapsedTimeCounter();
			}


			/**************************SIDE WALL*****************************/

			if (m_side.y < 1440)
			{
				m_side.x = (ofGetWindowWidth() / 2);
				m_side.y += 2;
			}
			else
				m_side.y = 0;


			/*******************************INK BLOTS**************************/
			if (m_inkFired) {
				m_keyTap = false;

				if (m_inkInit) {
					m_inkInit = false;
					m_colDiameter = 30;
					m_inkCollision = 0;
					m_inkOrigin = ofVec2f(m_object.x, m_object.y);
					m_inkOriginRot = (m_palmRotY * 2);
					for (int i = 0; i < 10; i++) {
						inkBlot[i].randomize();
					}

				}
				bool endInk = true;
				m_inkCollision -= 0.5;
				for (int i = 0; i < 10; i++) {
					inkBlot[i].pos.y -= inkBlot[i].speed;
					inkBlot[i].diamater += 0.5;
					if (inkBlot[i].opacity > 0) {
						endInk = false;
						inkBlot[i].opacity -= inkBlot[i].decay;
					}
					if (endInk) {
						m_inkFired = false;
					}
				}
			}

			else {
				m_keyTap = false;
			}


			/*****************************INK COLLISION******************************/
			if (m_inkFired) {
				inkHitBox = ofVec2f(m_inkOrigin.x, m_inkOrigin.y + m_inkCollision);

				m_colDiameter += 0.5;

				for (int i = 0; i < 10; i++) {

					float xCheck = enemy[i].pos.x - inkHitBox.x;
					if (xCheck < 0)
						xCheck *= -1;
					float yCheck = enemy[i].pos.y - inkHitBox.y;
					if (yCheck < 0)
						yCheck *= -1;

					if (xCheck < (5 + m_colDiameter) && yCheck < (5 + m_colDiameter)) {
						enemy[i].fishAnimState = 3;
						enemy[i].active = false;
					}
				
					
				}
			}
			for (int i = 0; i < 10; i++) {
				/******fish anim state******/
				if (enemy[i].fishAnimState == 1)
					enemy[i].fish.load("fish1.png");
				else if (enemy[i].fishAnimState == 2)
					enemy[i].fish.load("fish2.png");
				else if (enemy[i].fishAnimState == 3)
					enemy[i].fish.load("fish3.png");
			}
		}
	}
	if (ofGetKeyPressed(OF_KEY_RETURN)) {
		m_title = false;
	}
}


/***********************************************************************/
/********************************DRAW***********************************/
/***********************************************************************/





void ofApp::draw(){
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	ofSetFrameRate(60);
	ofClear(ofFloatColor(0.0f, 0.0f, 1.0f, 1.0f));
	ofSetCircleResolution(100);

	

	if (!m_dead) {

		/**********************DRAW BACKGROUND************************/
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
		m_background.draw(0, 0, 1280, 720);
		ofPopMatrix();

		/**********************DRAW ENEMIES************************/

		for (int i = 0; i < 10; i++) {
			ofPushMatrix();
				ofTranslate(enemy[i].pos);
				enemy[i].fish.draw(0, 0, 1);
			ofPopMatrix();
		}

		/*********************DRAW SIDES************************/
		ofPushMatrix();
		ofTranslate(m_side.x, m_side.y);
		m_sideImg.draw(0, 0, 1280, 1440);
		ofPopMatrix();

		ofPushMatrix();
		ofTranslate(m_side.x, m_side.y - 1440);
		m_sideImg.draw(0, 0, 1280, 1440);
		ofPopMatrix();



		/*********************DRAW POINT THING************************/
		if (!m_title) {
			ofPushMatrix();
			ofColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
			ofTranslate(m_Random);

			if (ofGetElapsedTimef() < 5)
			{
				ofScale(1 - 0.2*ofGetElapsedTimef(), 1 - 0.2*ofGetElapsedTimef());
				ofRotate(400 * ofGetElapsedTimef());
				ofSetColor(ofFloatColor(1.0f, 0.9f, 0.0f, 1.0f));
				ofDrawRectangle(0, 0, 40, 40);
				ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));

			}
			else {
				m_Random.x = ofRandom(0, ProjectConstants::WINDOW_SIZE_X);
				m_Random.y = ofRandom(0, ProjectConstants::WINDOW_SIZE_Y);
				m_health--;
				ofResetElapsedTimeCounter();
			}
			ofPopMatrix();
		}

		
		/**********************DRAW INK************************/

			if (m_inkFired) {
				for (int i = 0; i < 10; i++) {
					ofPushMatrix();
						ofTranslate(m_inkOrigin);
						ofRotate(m_inkOriginRot);
						ofPushMatrix();
							
							ofRotate(inkBlot[i].angle);
							ofTranslate(inkBlot[i].pos);
							ofSetColor(ofFloatColor(0.0f, 0.0f, 0.0f, inkBlot[i].opacity));
							ofDrawCircle(0,0, inkBlot[i].diamater);
							ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));

						ofPopMatrix();
					ofPopMatrix();
				}
				/*************Draw ink hitbox*************/
				/*ofNoFill();
				ofSetColor(ofFloatColor(0.0f, 1.0f, 0.0f, 1.0f));
				//ofDrawCircle(inkHitBox, m_colDiameter);
				ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
				ofFill();
				*/
			}

			/*****************DRAW PLAYER*********************/

			ofPushMatrix();
			ofColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
			ofTranslate(m_object.x, m_object.y);
			ofRotate(m_palmRotY * 2);
			//ofScale(m_object.z, m_object.z, m_object.z);
			m_shipImg.draw(0, 0, 100, 150);
			ofPopMatrix();


			


		/*********************DRAW HEALTHBAR************************/

		for (int i = 0; i < 10; i++) {
			if (i < m_health)
				ofSetColor(ofFloatColor(0.0f, 1.0f, 0.0f, 1.0f));
			else
				ofSetColor(ofFloatColor(1.0f, 0.0f, 0.0f, 1.0f));
			ofDrawRectangle(50 + i * 20, 50, 40, 40);
			ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		m_font.drawString("Points:" + ofToString(m_points), 100, 100);
	}
	/*********************DRAW DEATHSCREEN*************************/
	else {
		//ofClear(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
		//ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
		////ofScale(5, 5, 5);
		//m_font.drawString("You Died!", 50, 50);
		//m_font.drawString("Total Points:" + ofToString(m_points), 50, 100);

		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
		m_gameover.draw(0, 0, 1280, 720);
		m_font.drawString("Total Points:" + ofToString(m_points), -75, 100);
		//ofDrawBitmapString("Final Score:" + ofToString(m_points), -50, 100);
		ofPopMatrix();



	}
	/*********************DRAW TITLESCREEN*************************/
	if (m_title) {
		m_titleScreen.draw(ProjectConstants::WINDOW_SIZE_X / 2, ProjectConstants::WINDOW_SIZE_Y / 2, ProjectConstants::WINDOW_SIZE_X, ProjectConstants::WINDOW_SIZE_Y);

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
	//alternate controls
	/*m_inkFired = true;
	m_inkInit = true;*/
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::onLeapFrame(Leap::Frame frame){
	m_frame = frame; //so we can access frame data later (ie the update)
}
