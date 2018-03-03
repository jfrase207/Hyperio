#pragma once

#include "GameObject.h"

class Skybox : GameObject
{
public:
	Skybox();
	~Skybox();

	bool initialise();

	void draw();

private:
	GLuint texSkybox;
	GLuint vao;
	void create_skybox_mesh();
	void create_skybox_texture(const std::string & filePrefix, const std::string & fileType);
	bool load_side(GLuint texture, GLenum side_target, const std::string &file_name);
};