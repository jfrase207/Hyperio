#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <time.h>

//create a camera singleton
Camera *Camera::singleton;

//create the level grid for placement of asteroids
int levelgrid[40][10] =
{
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,1,0,1,1,0,1 },
	{ 0,0,1,1,0,0,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,1,1 },
	{ 0,1,1,0,0,0,1,1,0,0 },


};
//initial setup
MainGame::MainGame()
{
	_gameState = GameState::LOAD;
	Display* _gameDisplay = new Display(); //new display
	Audio* audioDevice();
}

MainGame::~MainGame()
{

}
//start the game
void MainGame::run()
{
	initSystems();
	gameLoop();
}

//Initialise display, sounds, models and camera.
void MainGame::initSystems()
{
	//this creates a display window for the game.
	_gameDisplay.initDisplay();

	//load sound file.
	backGroundMusic = audioDevice.loadSound("..\\res\\Sci-fi.wav");

	//Initialise the player and set transforms.
	player.init();
	player.setPosition(glm::vec3(75, 10, -100));
	player.rotate(glm::vec3(0, 1, 0), 90);

	//initialise the skybox
	skybox.initialise();

	//initialise the terrain
	terrain.init();
	
	//created asteroids and set transforms.
	for (int i = 0; i < 40; i++)
	{
		for (int o = 0; o < 10; o++)
		{
			if (levelgrid[i][o] != 0)
			{
				//create a new asteroid on each loop
				Asteroid *asteroid = new Asteroid();

				//initialise the asteroid and pick a random model to load from 3 availabel. Set transforms
				//and and to asteroids list.
				asteroid->init(rand() % 2 + 1);
				asteroid->setPosition(glm::vec3(15 * o, 10, 20 * i));
				asteroids.push_back(asteroid);
				
				//generate a random vec3 and add to list of random light directions for asteroid shader to use
				randX = (rand() % 100 + 30) / 10;
				randY = (rand() % 100 + 30) / 10;
				randZ = (rand() % 100 + 30) / 10;
				randomVec = glm::vec3(randX, randY, randZ);	
				randomLight.push_back(randomVec);
			}
		}
	}

	//initialise a camera and set default variables
	Camera::getSingleton().initCamera(glm::vec3(0, 2, -10), 45.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
	//set the camera to follow the player
	Camera::getSingleton().setFollowEntity(&player);
	//zero players initial move direction
	playerMovingDirection = 0;	
	
}

//resets game to initial state
void MainGame::reset()
{
	//reset all relevant variables to start game again
	player.init();
	player.setPosition(glm::vec3(75, 10, -100));	
	explosionCounter = 0;
	explosionRate = 0;
	gameLoop();
}

//Calculate the delta time
void MainGame::CalculateDeltaTime()
{
	//perform delta time calculations
	float timeSinceStart = clock();
	deltaTime = (timeSinceStart - oldTimeSinceStart) / 16;
	oldTimeSinceStart = timeSinceStart;
}

//start the gameloop
void MainGame::gameLoop()
{
	//perform these actions during gamestate load
	while (_gameState == GameState::LOAD)
	{
		//calculate delta time
		CalculateDeltaTime();

		//update camera
		Camera::getSingleton().update();

		//draw the game to screen
		drawGame();
		//process the players inputs
		processInput();

		//set shader explosion variable to zero
		explosionRate = 0;

		//start playing audio
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	while (_gameState == GameState::PLAY)
	{
		//calculate delta time
		CalculateDeltaTime();

		//loop check for collision between asteroids and player
		for (size_t i = 0; i < asteroids.size(); i++)
		{			
			//checking if player collison sphere is inside asteroid collision sphere 
			if (CollisionCheck(player.getSpherePos(), player.getSphereRadius(), asteroids[i]->getSpherePos(), asteroids[i]->getSphereRadius()))
			{
				//set player explosion variabale
				explosionRate = 0.2;
				//set gamestate to die
				_gameState = GameState::DIE;
				
			}

		}

		//process players inputs
		processInput();

		//move player forward on the z axis based on deltatime
		player.translate(glm::vec3(0, 0, 1),0.5 * deltaTime);		

		//move player left or right on the x depending on playermovingdirections variable -1 = left, + = right
		if (playerMovingDirection)
		{
			player.translate(glm::vec3(playerMovingDirection, 0, 0), 0.5 * deltaTime);
		}

		//update camera
		Camera::getSingleton().update();
		//draw game
		drawGame();
		//set audio to play and position of audio
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}
	//if game state is die then prevent player from moving but still draw game and recognise inputs
	while (_gameState == GameState::DIE)
	{
		//add explosionRate to explosionCounter. Zero if the is no collision
		explosionCounter += explosionRate;
		
		drawGame();
		processInput();
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

//process all the players keyboard inputs
void MainGame::processInput()
{
	//create an sdl event
	SDL_Event evnt;


	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			//in the event of sdl_quit change gamestate to exit
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
			//process the players keydown inputs
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:				
				break;
			case SDLK_s:				
				break;
			case SDLK_a:				
				playerMovingDirection = 1;				
				break;
			case SDLK_d:				
				playerMovingDirection = -1;				
				break;
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			case SDLK_RETURN:
				_gameState = GameState::PLAY;
				break;
			case SDLK_r:	
				reset();
				_gameState = GameState::LOAD;
				break;
			}
			break;
			
			//on key up reset playermoving direction to 0
		case SDL_KEYUP:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_a:
				if (playerMovingDirection == 1)
					playerMovingDirection = 0;

				break;
			case SDLK_d:
				if (playerMovingDirection == -1)
					playerMovingDirection = 0;
				break;
			}
			break;
		}
	}

}
//play audio
void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{

	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}
//collision function checks to see if objects are inside each otheres collison circles and if they are returns true
bool MainGame::CollisionCheck(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	//calculate the distance between 2 vectors
	float dist = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));
	
	//checks if distance is less than the radius of the 2 check spheres between objects
	if (dist < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//draw the game using opengl
void MainGame::drawGame()
{
	 //clear the screen before drawing
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);	
	//call the draw skybox function
	skybox.draw();
	//call the draw terrain function
	terrain.drawGrid();	
	//call the draw player function
	player.draw();	
	//set the players uniform variables for shader to use
	player.setGeo(explosionCounter, glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.2, 0.2, 1));	
	//call the draw asteroids function
	DrawAsteroids();		
	//swap the display buffers allows multi rendering
	_gameDisplay.swapBuffer();
}
//draw the asteroids
void MainGame::DrawAsteroids()
{
	//cycle through the asteroids list and draw to screen and set uniforms for shader to use
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->draw();		
		asteroids[i]->setFog(player.getPosition(),asteroids[i]->getPosition(),glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.5, 0.5, 0.5),300);		
	}
	
}