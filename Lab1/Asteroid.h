#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
	
public:
	Asteroid()
	{

	}

	virtual ~Asteroid()
	{

	}

	void init(int asteroidNum)
	{
		std::string filename = "..\\res\\Asteroid";
		filename += std::to_string(asteroidNum);
		filename += ".obj";
		mesh = new Mesh();
		mesh->loadModel(filename);
		texture = new Texture();
		texture->init("..\\res\\asteroid.jpg");
	    shader = new Shader();		
		shader->init(geo);
		this->rotate(glm::vec3(0, 1, 0), 180);
		sphereRadius = 5.0f;		
		
	   
	}
	
};