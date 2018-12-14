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
public:
	cPlayer();
	void update(double deltaTime);		// Player update method
}; 
#endif