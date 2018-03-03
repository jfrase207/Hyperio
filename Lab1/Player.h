#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player()
	{
		
	}

	void init()
	{
		mesh = new Mesh();
		mesh->loadModel("..\\res\\Ship.obj");
		texture = new Texture();
		texture->init("..\\res\\ship.png");
		shader = new Shader();
		shader->init("..\\res\\shader");
		this->rotate(glm::vec3(0, 1, 0), 180);
		sphereRadius = 2.5f;
	}

	virtual ~Player()
	{

	}
};