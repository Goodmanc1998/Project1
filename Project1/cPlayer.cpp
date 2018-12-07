/*
=================
cPlayer.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite()
{
	//this->rocketVelocity = 100;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{
	auto rads = PI / 180.0f * (this->getSpriteRotAngle() - 90.0f);

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (float)(cos(rads));
	direction.Y = (float)(sin(rads));

	SDL_Rect currentSpritePos = this->getSpritePos();
	
	//currentSpritePos.x += (int)(this->rocketVelocity * direction.X * this->move * deltaTime);
	//currentSpritePos.y -= (int)(this->rocketVelocity * direction.Y * this->move * deltaTime);

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
Sets the velocity for the rocket
=================================================================

void cPlayer::setRocketVelocity(int rocketVel)
{
	rocketVelocity = rocketVel;
}

=================================================================
Gets the rocket velocity
=================================================================
*/
/*int cPlayer::getRocketVelocity()
{
	//return rocketVelocity;
}
*/
/*
=================================================================
Sets the move value for the rocket
=================================================================
*/
void cPlayer::setRocketMove(int rocketMove)
{
	move = rocketMove;
}
/*
=================================================================
Gets the rocket move value
=================================================================
*/
int cPlayer::getRocketMove()
{
	return move;
}