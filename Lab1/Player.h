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
		shader->init("..\\res\\shaderToon");
		this->rotate(glm::vec3(0, 1, 0), 180);
		sphereRadius = 2.5f;
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
			setToon(glm::vec3(1, 1, 1), glm::vec3(0.2, 0.2, 1));
		}

		if (texture)
			texture->Bind(0);

		if (mesh)
		{
			mesh->draw();
			mesh->updateSphereData(this->position, sphereRadius);
		}
	}

	void setToon(glm::vec3 lightDir, glm::vec3 color)
	{
		shader->setVec3("lightDir", lightDir);
		shader->setVec3("_color", color);
		
	}

	virtual ~Player()
	{

	}
};