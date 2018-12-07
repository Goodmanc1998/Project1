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
	//currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime);
	//currentSpritePos.y += (int)(this->getSpriteTranslation().y * deltaTime);


	//Movement of the Missiles
	currentSpritePos.y += missileVelocity; // * missileVelocity

	//cout << currentSpritePos.y << endl;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());

	if (currentSpritePos.y > 600)
	{
		//this->setActive(false);	
		//this->setSpritePos({ currentSpritePos.x, -100 });

//		void.MissileDestroy();
		
	}
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
