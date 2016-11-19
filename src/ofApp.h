#pragma once

#include "ofMain.h"
#include "ofxLeap.h"

#include "ProjectConstants.h"
#include "ink.h"
#include "enemy.h"


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
		
		void onLeapFrame(Leap::Frame frame); //an arbitrary function where our leap frame is updated

		ofxLeapMotion::Device m_device; //reference to leap device
		Leap::Frame m_frame;			//a frame contains all leapmotion data at some instance of time
		ofVec3f m_palmPos;				//a var that we will save palm pos to
		float m_palmRotY;				//yaw
	

	//	ofVec2f m_enemy[10];
	//	float m_eSpeed[10];
		
		ofTrueTypeFont	m_font;
		
		float m_health;
		float m_points;
		bool m_dead;

		ofVec2f m_Random;
		ofVec3f m_object;

		ofImage	m_shipImg;
		ofImage	m_titleScreen;
		int counter;
		bool switchState;

		bool m_title;

		bool m_keyTap;
		bool m_inkFired;
		bool m_inkInit;

		float m_inkCollision;
		float m_colDiameter;
		ofVec2f inkHitBox;

		ofVec2f m_inkOrigin;
		float m_inkOriginRot;

		ink inkBlot[10];
		enemy enemy[10];

		ofImage	m_sideImg;
		ofVec4f m_side;

		ofImage	m_background;

		ofImage m_gameover;
};
