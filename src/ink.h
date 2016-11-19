#pragma once
#include "ofMain.h"


class ink
{
public:

	ofVec2f pos;
	float diamater;
	float angle;
	float speed;
	float decay;
	float opacity;

	ink();
	~ink();

	void randomize();
};

