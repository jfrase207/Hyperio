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
		shader->init(geo,geo);
		this->rotate(glm::vec3(0, 1, 0), 90);
		sphereRadius = 2.5f;	
		
		
	}
	
	

	virtual ~Player()
	{

	}
};