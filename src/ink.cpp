#pragma once

#include "ink.h"

ink::ink() {
	
}

ink::~ink() {


}

void ink::randomize() {
	angle = ofRandom(-30, 30);
	decay = ofRandom(0.005, 0.01);
	diamater = ofRandom(1, 5);
	pos = ofVec2f(0, 0);
	speed = ofRandom(0.1f, 1.0f);
	opacity = 1.0f;

}