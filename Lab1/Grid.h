#pragma once
#include "GameObject.h"



class Grid : public GameObject
{
public:

	struct Vert
	{
		float x, y, z;
	};
	
	Grid();
	~Grid();
	bool init();	
	void drawGrid();
	void GenerateGrid();

	std::vector<Vert> vertices;
	std::vector<float> indices;

	GLuint VAO;
	GLuint VBO;

	void GridMesh();

	GLuint vao;

};