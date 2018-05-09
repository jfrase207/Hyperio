#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <time.h>

Transform transform;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera *Camera::singleton;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
float deltaTime = 0;
float oldTimeSinceStart = 0;
float counter = 0;
float multiCount = 0;
float randX, randY, randZ;

glm::vec3 randomVec;



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

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Audio* audioDevice();
}

MainGame::~MainGame()
{

}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay();

	backGroundMusic = audioDevice.loadSound("..\\res\\Sci-fi.wav");

	

	player.init();
	player.setPosition(glm::vec3(75, 0, -100));
	player.rotate(glm::vec3(0, 1, 0), 90);

	/*for (int i = 0; i < 40; i++)
	{
		for (int o = 0; o < 10; o++)
		{
			if (levelgrid[i][o] != 0)
			{
				Asteroid *asteroid = new Asteroid();
				asteroid->init(rand() % 2 + 1);
				asteroid->setPosition(glm::vec3(15 * o, 0, 20 * i));
				asteroids.push_back(asteroid);
				

				randX = (rand() % 100 + 30) / 10;
				randY = (rand() % 100 + 30) / 10;
				randZ = (rand() % 100 + 30) / 10;
				randomVec = glm::vec3(randX, randY, randZ);	
				randomLight.push_back(randomVec);
			}
		}
	}*/
	Camera::getSingleton().initCamera(glm::vec3(0, 2, -10), 45.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 500.0f);
	
	playerMovingDirection = 0;
	
	Camera::getSingleton().setFollowEntity(&player);
	
	skybox.initialise();	
	grid.init();
}

void MainGame::reset()
{
	player.init();
	player.setPosition(glm::vec3(75, 0, -100));	
	counter = 0;
	multiCount = 0;
	gameLoop();
}

void MainGame::gameLoop()
{
	_gameState = GameState::LOAD;
	while (_gameState == GameState::LOAD)
	{
		float timeSinceStart = clock();
		deltaTime = (timeSinceStart - oldTimeSinceStart) / 16;
		oldTimeSinceStart = timeSinceStart;

		Camera::getSingleton().update();

		drawGame();
		processInput();
		multiCount = 0;
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	while (_gameState == GameState::PLAY)
	{
		//LAST = NOW;
		//NOW = SDL_GetPerformanceCounter();

		//deltaTime = (float)(NOW - LAST) * 1000.0 / (float)SDL_GetPerformanceFrequency();



		float timeSinceStart = clock();
		deltaTime = (timeSinceStart - oldTimeSinceStart)/16;
		oldTimeSinceStart = timeSinceStart;

		//if (deltaTime > 1)
			//continue;		

		for (size_t i = 0; i < asteroids.size(); i++)
		{
			if (collision(player.getSpherePos(), player.getSphereRadius(), asteroids[i]->getSpherePos(), asteroids[i]->getSphereRadius()))
			{
				multiCount = 0.2;
				_gameState = GameState::DIE;
				
			}

		}

		processInput();

		player.translate(glm::vec3(0, 0, 1),0.5 * deltaTime);		

		if (playerMovingDirection)
		{
			player.translate(glm::vec3(playerMovingDirection, 0, 0), 0.5 * deltaTime);
		}

		Camera::getSingleton().update();

		drawGame();

		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	while (_gameState == GameState::DIE)
	{
		drawGame();
		processInput();
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:
				player.translate(glm::vec3(0, 0, 1),0.2);
				break;
			case SDLK_s:
				player.translate(glm::vec3(0, 0, 1), -0.2);
				break;
			case SDLK_a:				
				playerMovingDirection = 1;
				player.translate(glm::vec3(1, 0, 0), 0.2);
				break;
			case SDLK_d:				
				playerMovingDirection = -1;
				player.translate(glm::vec3(1, 0, 1), -0.2);
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
			case SDLK_f:
				Camera::getSingleton().setPosition(glm::vec3(0,0,100));
				break;
			}
			break;

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

bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::drawGame()
{
	 counter += multiCount;
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	
	skybox.draw();

	grid.drawGrid();	
	
	
	player.draw();	
	
	player.setGeo(counter, glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.2, 0.2, 1));
	

	//DrawAsteroids();		

	_gameDisplay.swapBuffer();
}

void MainGame::DrawAsteroids()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		
		asteroids[i]->draw();
		asteroids[i]->setFog(player.getPosition(),asteroids[i]->getPosition(),glm::vec3(0.8, 0.8, 0.8),randomLight[i]);
		
	}
	
}





void MainGame::DrawTriangle()
{
	
	
	
}
