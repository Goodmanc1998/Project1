/*
=================
cButton.h
- Header file for class definition - SPECIFICATION
=================
*/
#ifndef _CBUTTON_H
#define _CBUTTON_H
#include "cSprite.h"

class cButton : public cSprite
{

private:

	SDL_Point buttonClickedRC;
	bool clicked = false;

public:
	cButton();

	gameState update(gameState theCurrentGameState, gameState newGameState, SDL_Point theAreaClicked);

	bool getClicked();
	void setClicked(bool state);

};
#endif