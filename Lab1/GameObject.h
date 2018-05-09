#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "transform.h"
#include <vector>
#include <GLFW\glfw3.h>
#include <time.h>

class GameObject : public Entity
{
	Transform transform;
protected:
	Mesh *mesh;
	Texture *texture; // Only 1 texture per mesh
	Shader *shader;

	float sphereRadius;

public:

	const std::string& rimToon = "..\\res\\shaderRimToon";
	const std::string& toon = "..\\res\\shaderToon";
	const std::string& fog = "..\\res\\shaderFog";
	const std::string& blur = "..\\res\\shaderBlur";
	const std::string& test = "..\\res\\shaderTest";
	const std::string& geo = "..\\res\\geometryShader";
	const std::string& grid = "..\\res\\grid";
	const std::string& skybox = "..\\res\\skybox";


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

	void setToon(glm::vec3 lightDir, glm::vec3 _color)
	{
		draw();
		shader->setVec3("lightDir", lightDir);
		shader->setVec3("_color", _color);

	}

	void setRimToon(glm::vec3 lightDir,glm::vec3 _color)
	{
		draw();
		shader->setVec3("_color", _color);
		shader->setVec3("lightDir", lightDir);
		shader->setMat4("u_vm", Camera::getSingleton().GetView());
		shader->setMat4("u_pm", Camera::getSingleton().GetProjection());
		shader->setMat4("v_pos", transform.GetModel());
	}

	void setFog(glm::vec3 zPosPlayer, glm::vec3 zPosAsteroid, glm::vec3 _color, glm::vec3 _lightDir)
	{
		
		shader->setVec3("lightDir", _lightDir);
		shader->setVec3("_color", _color);
		//shader->setMat4("u_vm", Camera::getSingleton().GetView());
		//shader->setMat4("u_pm", Camera::getSingleton().GetProjection());
		
		shader->setVec3("fogColor", glm::vec3(0.2, 0.2, 0.2));
		shader->setFloat("minDist", -150);
		shader->setFloat("maxDist", 150);

		shader->setVec3("zposplayer", zPosPlayer);
		shader->setVec3("zposasteroid", zPosAsteroid);
	

	}

	void setBlur()
	{
		shader->setVec4("OuterColor", glm::vec4(1, 0, 0, 1));
		shader->setVec4("InnerColor", glm::vec4(1, 1, 1, 1));
		shader->setFloat("RadiusInner", 1);
		shader->setFloat("RadiusOuter", 10);

	}	

	
	void setGeo(float mag, glm::vec3 lightDir, glm::vec3 _color)
	{	

		shader->setFloat("time", glfwGetTime());
		shader->setFloat("mag", mag);
		shader->setVec3("_color", _color);
		shader->setVec3("lightDir", lightDir);
	}

	void setMvp(Transform transform, Camera camera)
	{
		glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel();
		shader->setMat4("_mvp", mvp);
	}

};
