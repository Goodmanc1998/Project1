/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cAsteroid::cAsteroid() : cSprite()
{
	this->asteroidVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cAsteroid::update(double deltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();
	//currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime);
	//currentSpritePos.y += (int)(this->getSpriteTranslation().y * deltaTime);


	//Movement of the Missiles
	currentSpritePos.y += (currentSpritePos.y + 30) * deltaTime; // * asteroidVelocity

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());

}


/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(int AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
int cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}
