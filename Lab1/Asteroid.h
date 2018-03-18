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
		shader->init("..\\res\\shaderToon");		
		
		this->rotate(glm::vec3(0, 1, 0), 180);
		sphereRadius = 5.0f;
	}

	virtual void DrawPhase()
	{
		Transform transform;
		transform.SetPos(this->position);
		transform.SetRot(this->rotation);
		transform.SetScale(glm::vec3(1, 1, 1));

		if (shader)
		{
			shader->Bind();
			shader->Update(transform, Camera::getSingleton());
			setToon(glm::vec3(0.2, 0.8, 0.5), glm::vec3(0.5, 0.5, 0.5));
		}

		if (texture)
			texture->Bind(0);

		if (mesh)
		{
			mesh->draw();
			mesh->updateSphereData(this->position, sphereRadius);
		}
	}

	void setToon(glm::vec3 lightDir,glm::vec3 color)
	{
		shader->setVec3("lightDir", lightDir);
		shader->setVec3("_color", color);
	}
};