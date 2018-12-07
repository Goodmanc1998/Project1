#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>
#include <random>
// Game specific includes
#include "missileControlGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();


	static cGame* getInstance();


	int missileDestroyed; // Used to count the amout of missiles the player destroys

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;

	bool loop;

	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cPlayer thePlayer;
	cMissile theMissile;
	cBullet theBullet;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cMissile*> theMissiles;
	vector<cBullet*> theBullets;
	vector<cSprite*> theExplosions;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Game objects
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;


	
	gameState theGameState; 
	cTexture* tempTextTexture;

	SDL_Rect pos;
	FPoint scale;

	//Used for buttons
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	SDL_Point theAreaClicked;
	btnTypes theBtnType;


	//Used to save players score and lives
	int theScore;
	int lifes;
	// Used to turn theScore and lifes into string variables
	string strScore;
	string strLifes;

	// Used to run if statement to update score and life changes
	bool scoreChange = false;
	bool lifeChange = false;

};
#endif
