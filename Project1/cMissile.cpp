/*
=================
cMissile.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cMissile.h"
#include "cGame.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cMissile::cMissile() : cSprite()
{
	this->missileVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cMissile::update(double deltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();

	//Movement of the Missiles
	currentSpritePos.y += missileVelocity; 


	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}


/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cMissile::setmissileVelocity(int AsteroidVel)
{
	missileVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
int cMissile::getmissileVelocity()
{
	return missileVelocity;
}
