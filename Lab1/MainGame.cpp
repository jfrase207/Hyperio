#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>

Transform transform;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera *Camera::singleton;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
float deltaTime = 0;

int levelgrid[5][10] =
{
	{ 1,0,0,1,1,1,0,1,0,1 },
	{ 0,1,1,0,1,0,1,0,1,0 },
	{ 0,0,1,1,0,1,1,0,1,0 },
	{ 1,0,0,1,1,0,0,1,0,1 },
	{ 0,1,1,0,0,1,1,0,1,1 },
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
	player.setPosition(glm::vec3(75, 0, -200));

	for (int i = 0; i < 5; i++)
	{
		for (int o = 0; o < 10; o++)
		{
			if (levelgrid[i][o] != 0)
			{
				Asteroid *asteroid = new Asteroid();
				asteroid->init(rand() % 2 + 1);
				asteroid->setPosition(glm::vec3(15 * o, 0, 20 * i));
				asteroids.push_back(asteroid);
			}
		}
	}
	Camera::getSingleton().initCamera(glm::vec3(0, 2, -10), 45.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
	counter = 1.0f;
	playerMovingDirection = 0;
	Camera::getSingleton().setFollowEntity(&player);
	skybox.initialise();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (float)(NOW - LAST) * 1000.0 / (float)SDL_GetPerformanceFrequency();

		if (deltaTime > 1)
			continue;

		for (size_t i = 0; i < asteroids.size(); i++)
		{
			if (collision(player.getSpherePos(), player.getSphereRadius(), asteroids[i]->getSpherePos(), asteroids[i]->getSphereRadius()))
				_gameState = GameState::EXIT;
		}

		processInput();

		player.translate(glm::vec3(0, 0, 1), 0.05f * (float)deltaTime);

		if (playerMovingDirection)
		{
			player.translate(glm::vec3(playerMovingDirection, 0, 0), 0.03 * deltaTime);
		}

		Camera::getSingleton().update();

		drawGame();

		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
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
					Camera::getSingleton().translate(glm::vec3(0, 0, 1), 3 * deltaTime);
					break;
				case SDLK_s:
					Camera::getSingleton().translate(glm::vec3(0, 0, -1), 3 * deltaTime);
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
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	skybox.draw();

	player.draw();
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->draw();
	}

	counter = counter + 0.01f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	

	_gameDisplay.swapBuffer();
} 