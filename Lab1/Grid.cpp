#include "Grid.h"



Transform transformer;
float gridSizeX = 5;
float gridSizeY = 5;
float* gridVerts = NULL;
float* ibData = NULL;
int gridsize;
int vertCount = 3;



Grid::Grid()
{

}

Grid::~Grid()
{

}

bool Grid::init()
{
	GridMesh();
	return true;
}

void Grid::drawGrid()
{
	
	transformer.SetPos(glm::vec3(75, 0, -100));
	transformer.SetRot(this->rotation);
	transformer.SetScale(glm::vec3(1, 1, 1));
	
	

	if (shader)
	{
		shader->Bind();
		shader->Update(transformer, Camera::getSingleton());
	}

	float randY = (rand() % 10 + 1) / 10;

	
	

	glDisable(GL_CULL_FACE);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	
	glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT,0);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);


	//glCullFace(GL_BACK);

	
}

void Grid::GridMesh()
{
	

	gridsize = (gridSizeX * gridSizeY);
	gridVerts = new float[gridsize];

	ibData = new float[gridSizeX * gridSizeY];

	int i = 0;
	int offset = 0;

	//for (float z = 0; z < gridSizeY; z++)
	//{
	//	for (float x = 0; x < gridSizeX; x++)
	//	{

	//		Vert vert;

	//		vert.x = x;
	//		vert.y = 0;
	//		vert.z = z + 1;

	//		vertices.push_back(vert);			

	//		vert.x = x;
	//		vert.y = 0;
	//		vert.z = z;

	//		vertices.push_back(vert);


	//		vert.x = x + 1;
	//		vert.y = 0;
	//		vert.z = z + 1;

	//		vertices.push_back(vert);

	//		vert.x = x + 1;
	//		vert.y = 0;
	//		vert.z = z;


	//		vertices.push_back(vert);

	//		i += 4;

	//	}
	//}

	for (float z = 0; z < gridSizeY; z++)
	{
		for (float x = 0; x < gridSizeX; x++)
		{

			Vert vert;

			vert.x = x;
			vert.y = 0;
			vert.z = z;

			vertices.push_back(vert);

		}
	}
	
	for (size_t in = 0; in <  19 ; in++)
	{
		float indices1 = in;

		indices.push_back(indices1);

	}



	GLuint vbo;
	GLuint ibo;

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//gridVerts = 0;
	//ibData = 0;

	shader = new Shader();
	shader->init(grid);

	
}

void generateIBdata()
{
	

}


