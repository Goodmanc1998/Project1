/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include "time.h"
#include <Windows.h>
#include <stdlib.h>


cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);


	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> AsteroidDis{ 9, 1014 };
	uniform_int_distribution<> AsteroidTextDis{ 0, 0 };


	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;


	// Store the textures
	textureName = { "missile", "bullet","theRocket","theBackground", "explosion" };
	texturesToUse = { "Images\\Sprites\\Missile.png", "Images\\Sprites\\bullet.png", "Images\\Sprites\\cannonSprite.png", "Images\\Bkg\\Background.jpg", "Images\\Sprites\\explosion.png" };
	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	tempTextTexture = theTextureMgr->getTexture("theBackground");
	//Button Code


	theGameState = gameState::playing;

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}


	gameTextList = { "Missile Control", "Score : " };

	theScore = 0;
	strScore = gameTextList[1];
	strScore += to_string(theScore).c_str();


	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));


	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}


	theSoundMgr->getSnd("theme")->play(-1);


	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());


	theRocket.setSpritePos({ 450, 600 }); // Setting the sprites rotation
	theRocket.setSpriteRotAngle({ -90 }); // Setting the sprites angle (Stright up)
	theRocket.setTexture(theTextureMgr->getTexture("theRocket")); // Getting the texture of the rocket
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	//theRocket.setSpriteTranslation({ 100, 100 });


	// Create vector array of textures

		for (int astro = 0; astro < 2; astro++)
		{

			theAsteroids.push_back(new cAsteroid);
			theAsteroids[astro]->setSpritePos({ AsteroidDis(gen), 0 }); // The X and Y cooridinates to spawn the missiles
			theAsteroids[astro]->setSpriteTranslation({ 0, -50 });
			int randAsteroid = AsteroidTextDis(gen);
			theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
			theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
			theAsteroids[astro]->setAsteroidVelocity(25);
			theAsteroids[astro]->setActive(true);
		}
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);

	switch (theGameState)
	{
	case gameState::playing:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		

	}
	}
	
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);


}

void cGame::update()
{


}

void cGame::update(double deltaTime)
{	
		// Update the visibility and position of each asteriod
		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				++asteroidIterator;
			}
		}
		// Update the visibility and position of each bullet
		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}
		}
		// Update the visibility and position of each explosion
		vector<cSprite*>::iterator expIterartor = theExplosions.begin();
		while (expIterartor != theExplosions.end())
		{
			if ((*expIterartor)->isActive() == false)
			{
				expIterartor = theExplosions.erase(expIterartor);
			}
			else
			{
				(*expIterartor)->animate(deltaTime);
				++expIterartor;
			}
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/

		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			(*bulletIterartor)->update(deltaTime);
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*asteroidIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theExplosions.push_back(new cSprite);
					int index = theExplosions.size() - 1;
					theExplosions[index]->setSpriteTranslation({ 0, 0 });
					theExplosions[index]->setActive(true);
					theExplosions[index]->setNoFrames(16);
					theExplosions[index]->setTexture(theTextureMgr->getTexture("explosion"));
					theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("explosion")->getTWidth() / theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("explosion")->getTHeight());
					theExplosions[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x + (int)((*asteroidIterator)->getSpritePos().w / 2), (*asteroidIterator)->getSpritePos().y + (int)((*asteroidIterator)->getSpritePos().h / 2) });

					theSoundMgr->getSnd("explosion")->play(0);



					// Lab 7 code goes here

				}
			}
		}
		// Update the Rockets position
		//theRocket.update(deltaTime);


	
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				break;
				case SDLK_RIGHT:
				{
					theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() + 2.5);
				}
				break;

				case SDLK_LEFT:
				{
					theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() - 2.5);
				}
				break;
				case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ theRocket.getSpritePos().x, theRocket.getSpritePos().y });
					//cout << "sprite pos " << theRocket.getSpritePos().x << " sprite dimension " << theRocket.getSpriteDimensions().w << " added " << theRocket.getSpritePos().x + (theRocket.getSpriteDimensions().w / 2) <<  endl;
					//theBullets[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 5, 5});
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity(50);
					theBullets[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << theRocket.getBoundingRect().x << " y: " << theRocket.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;
				default:
					break;
				}

			default:
				break;
		}
		

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

