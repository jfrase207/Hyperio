#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Player.h"
#include "transform.h"
#include "Asteroid.h"
#include "Skybox.h"
#include "Grid.h"
#include "Audio.h"

enum class GameState{LOAD,PLAY,DIE,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void reset();
	void processInput();
	void gameLoop();
	void drawGame();
	void DrawAsteroids();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void DrawTriangle();

	Display _gameDisplay;
	GameState _gameState;

	Audio audioDevice;
	Player player;
	Grid grid;
	std::vector<Asteroid *> asteroids;
	std::vector<glm::vec3> randomLight;
	int playerMovingDirection;
	Skybox skybox;
	float counter;
	unsigned int backGroundMusic;
};

