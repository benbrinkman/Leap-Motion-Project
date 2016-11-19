#pragma once
#include "ofMain.h"


class enemy
{
public:

	ofVec2f pos;
	float speed;
	bool active;
	ofImage	fish;
	float fishAnimState;
	enemy();
	~enemy();

};

