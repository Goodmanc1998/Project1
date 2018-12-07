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
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

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

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();


	//Setting the area click to top left of the screen
	theAreaClicked = {0, 0};

	theScore = 0;
	missileDestroyed = 0;
	lifes = 5;
	
	// Store the textures
	textureName = { "missile", "bullet","thePlayer","theBackground", "explosion", "keys" };
	texturesToUse = { "Images\\Sprites\\Missile.png", "Images\\Sprites\\bullet.png", "Images\\Sprites\\cannonSprite.png", "Images\\Bkg\\Background.jpg", "Images\\Sprites\\explosion.png", "Images\\Sprites\\Keys.png" };
	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Create textures for Game Dialogue (text)
	fontList = {"METALORD" };
	fontsToUse = {"Fonts/METALORD.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	//Creating the Textures for Buttons
	btnNameList = { "exit_btn", "menu_btn", "play_btn"};
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png"};
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 275 }};
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	//Setting the Game Sate to Menu
	theGameState = gameState::menu;

	theBtnType = btnTypes::exit;




	// Creating text that will be used in the game
	gameTextList = { "Missile Control", "Score : ","Arrow Keys - Rotate Space - Shoot", "Game Over", "Lifes : " };

	strScore = gameTextList[1];
	strScore += to_string(theScore).c_str();

	strLifes = gameTextList[4];
	strLifes += to_string(lifes).c_str();

	
	//cTexture* tempTextText
	theTextureMgr->addTexture("Title", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, gameTextList[0], textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("controls", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, gameTextList[2], textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("gameOver", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, gameTextList[3], textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("lifes", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, strLifes.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));


	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/Theme.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	thePlayer.setSpritePos({ 450, 650 }); // Setting the sprites rotation
	thePlayer.setSpriteRotAngle({ -90 }); // Setting the sprites angle (Stright up)
	thePlayer.setTexture(theTextureMgr->getTexture("thePlayer")); // Getting the texture of the rocket
	thePlayer.setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());


	// Create vector array of textures

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

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

	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> AsteroidDis{ 24, 1000 };
	uniform_int_distribution<> AsteroidTextDis{ 0, 0 };

	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		//Renders in the required text
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { (WINDOW_WIDTH / 2 ) - (theTextureMgr->getTexture("Title")->getTWidth() / 2), 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("controls");
		pos = { (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("controls")->getTWidth() / 2), 550, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("keys");
		pos = { (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("keys")->getTWidth() / 2), 600, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Button Code Goes Below
		theButtonMgr->getBtn("play_btn")->setSpritePos({ (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("play_btn")->getTWidth() / 2), 275 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("exit_btn")->getTWidth() / 2), 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());


	}
	break;
	case gameState::playing:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		if (lifeChange)
		{
			theTextureMgr->deleteTexture("lifes");
			theTextureMgr->addTexture("lifes", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, strLifes.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

			lifeChange = false;
		}


		if (scoreChange)
		{
			theTextureMgr->deleteTexture("theScore");
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("METALORD")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
			
			scoreChange = false;
		}

		//Rendering the Score
		tempTextTexture = theTextureMgr->getTexture("theScore");
		pos = { 700, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("lifes");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Rendering the Rocket
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());

		int numEn = theMissiles.size();
		for (int astro = numEn; astro <  4 ; astro++)
		{
			theMissiles.push_back(new cMissile);
			theMissiles[astro]->setSpritePos({ AsteroidDis(gen), -75 }); // The X and Y cooridinates to spawn the missiles
			theMissiles[astro]->setSpriteTranslation({ 0, -5 });
			int randAsteroid = AsteroidTextDis(gen);
			theMissiles[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
			theMissiles[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());

			theMissiles[astro]->setmissileVelocity(2.5);
			theMissiles[astro]->setActive(true);
		}

		// Render each asteroid in the vector array
		for (int draw = 0; draw < (int)theMissiles.size(); draw++)
		{
			theMissiles[draw]->render(theRenderer, &theMissiles[draw]->getSpriteDimensions(), &theMissiles[draw]->getSpritePos(), theMissiles[draw]->getSpriteRotAngle(), &theMissiles[draw]->getSpriteCentre(), theMissiles[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
		// Render each explosion in the vector array
		for (int draw = 0; draw < (int)theExplosions.size(); draw++)
		{
			theExplosions[draw]->render(theRenderer, &theExplosions[draw]->getSourceRect(), &theExplosions[draw]->getSpritePos(), theExplosions[draw]->getSpriteScale());
		}

		theAreaClicked={ 0,0 };


	}
	break;
	case gameState::end:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("Title")->getTWidth() / 2), 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("menu_btn")->getTWidth() / 2), 275 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());


		tempTextTexture = theTextureMgr->getTexture("theScore");
		pos = { (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("theScore")->getTWidth() / 2), 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);


		theButtonMgr->getBtn("exit_btn")->setSpritePos({ (WINDOW_WIDTH / 2) - (theTextureMgr->getTexture("exit_btn")->getTWidth() / 2), 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		

	}
	break;
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}


	SDL_RenderPresent(theRenderer);
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
	// Check Button clicked and change state

	theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);


	
	if (theGameState != gameState::playing) 
	{
		lifes = 5;

		strLifes = gameTextList[4];
		strLifes += to_string(lifes).c_str();

		lifeChange = true;

	}

	if (theGameState == gameState::menu)
	{
		theScore = 0;

		strScore = gameTextList[1];
		strScore += to_string(theScore).c_str();

		scoreChange = true;

		missileDestroyed = 0;
	}


	vector<cMissile*>::iterator asteroidIterator = theMissiles.begin();

	if (theGameState == gameState::playing)
	{
		if (lifes <= 0 || missileDestroyed == 50)
		{
			// Update the visibility and position of each asteriod
			while (asteroidIterator != theMissiles.end())
				asteroidIterator = theMissiles.erase(asteroidIterator);

			theGameState = gameState::end;
		}
	}

	while (asteroidIterator != theMissiles.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theMissiles.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);

			//Checking if the missiles move below the screen to respawn them at the top of the screen.
			if ((*asteroidIterator)->getSpritePos().y > 750 )
			{
				(*asteroidIterator)->setSpritePos({(*asteroidIterator)->getSpritePos().x, -100 });

				scoreChange = true;
				theScore = theScore - 5;

				strScore = gameTextList[1];
				strScore += to_string(theScore).c_str();
				
				cout << "Score is " << theScore << endl;

				lifeChange = true;
				lifes = lifes --;

				strLifes = gameTextList[4];
				strLifes += to_string(lifes).c_str();




			}
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
		for (vector<cMissile*>::iterator asteroidIterator = theMissiles.begin(); asteroidIterator != theMissiles.end(); ++asteroidIterator)
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

				scoreChange = true;
				theScore = theScore + 10;
				
				strScore = gameTextList[1];
				strScore += to_string(theScore).c_str();


				cout << "Score is " << theScore << endl;

				missileDestroyed = missileDestroyed + 1;

				cout << "Missile Destroyed is " << missileDestroyed << endl;
				

			}
		}
	}

	// Update the Rockets position
	thePlayer.update(deltaTime);
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
		case SDL_MOUSEBUTTONDOWN:
		{
		case SDL_BUTTON_LEFT:
		{
			if (theGameState != gameState::playing)
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
		}
		break;
		}
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;
				break;
			case SDLK_RIGHT:
			{
				if (theGameState == gameState::playing)
				{
					thePlayer.setSpriteRotAngle(thePlayer.getSpriteRotAngle() + 5);

				}
			}
			break;

			case SDLK_LEFT:
			{
				if (theGameState == gameState::playing)
				{
					thePlayer.setSpriteRotAngle(thePlayer.getSpriteRotAngle() - 5);
				}
			}
			break;
			case SDLK_SPACE:
			{

				if (theGameState == gameState::playing)
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ thePlayer.getSpritePos().x + thePlayer.getSpriteCentre().x - 7, thePlayer.getSpritePos().y + thePlayer.getSpriteCentre().y });
					//cout << "sprite pos " << thePlayer.getSpritePos().x << " sprite dimension " << thePlayer.getSpriteDimensions().w << " added " << thePlayer.getSpritePos().x + (thePlayer.getSpriteDimensions().w / 2) <<  endl;
					theBullets[numBullets]->setSpriteTranslation({ 5, 5 });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity(75);
					theBullets[numBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;
					
					theSoundMgr->getSnd("shot")->play(0);
				}
			}
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