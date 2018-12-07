/*
=================
cPlayer.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _cPlayer_H
#define _cPlayer_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	int rocketVelocity;
	int move;

public:
	cPlayer();
	void update(double deltaTime);		// Player update method
	void setRocketMove(int rocketMove);   // Sets the rocket move Value
	int getRocketMove();				 // Gets the rocket move value
}; 
#endif