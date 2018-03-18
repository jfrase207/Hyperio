#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "transform.h"
#include <vector>

class GameObject : public Entity
{
protected:
	Mesh *mesh;
	Texture *texture; // Only 1 texture per mesh
	Shader *shader;
	float sphereRadius;

public:
	GameObject()
	{
		mesh = 0;
		texture = 0;
		shader = 0;
		sphereRadius = 1.0f;
	}

	virtual ~GameObject()
	{
		if (mesh)
		{
			delete mesh;
			mesh = 0;
		}

		if (texture)
		{
			delete texture;
			texture = 0;
		}

		if (shader)
		{
			delete shader;
			shader = 0;
		}
	}

	virtual void draw()
	{
		Transform transform;
		transform.SetPos(this->position);
		transform.SetRot(this->rotation);
		transform.SetScale(glm::vec3(1, 1, 1));

		if (shader)
		{
			shader->Bind();
			shader->Update(transform, Camera::getSingleton());

			
		}

		if (texture)
			texture->Bind(0);

		if (mesh)
		{
			mesh->draw();
			mesh->updateSphereData(this->position, sphereRadius);
		}
	}

	glm::vec3 getSpherePos()
	{
		return mesh->getSpherePos();
	}

	float getSphereRadius()
	{
		return mesh->getSphereRadius();
	}		
};
