/*
=================
cMissile.h
- Header file for class definition - SPECIFICATION
- Header file for the missile class which is a child of cSprite class
=================
*/
#ifndef _cMissile_H
#define _cMissile_H
#include "cSprite.h"


class cMissile : public cSprite
{
private:
	int missileVelocity;

public:
	cMissile();
	void update(double deltaTime);		// Player update method
	void setmissileVelocity(int asteroidVel);   // Sets the velocity for the missile
	int getmissileVelocity();				 // Gets the missile velocity
};
#endif