#include "Grid.h"
#include "PerlinNoise.h"



Transform transformer;
int width = 200;
int height = 200;
glm::vec3 scale = glm::vec3(1, 1, 1);
float* vertices = 0;
int* indices = 0;

Grid::Grid()
{

}

Grid::~Grid()
{

}

bool Grid::init()
{
	
	getVertices(width,height);
	getIndices(width,height);
	InitiateBuffers();
	return true;
}


int getVerticesCount(int width, int height) {
	return width * height * 3;
}

int getIndicesCount(int width, int height) {
	return (width*height) + (width - 1)*(height - 2);
}

bool Grid::getVertices(int width, int height) {
	

	vertices = new float[getVerticesCount(width, height)];
	int i = 0;

	unsigned int seed = (rand() % 1000 + 1);
	PerlinNoise pn(seed);

	for (int row = 0; row<width; row++) {
		for (int col = 0; col<height; col++) {

			double x = (double)row / ((double)width);
			double y = (double)col / ((double)height);

			float n = pn.noise(10 * x, 10 * y, 0.8);
			float yVer = n * 20;

			vertices[i++] = (float)row;
			vertices[i++] = yVer;
			vertices[i++] = (float)col;
		}
	}

	return true;
}

bool Grid::getIndices(int width, int height) {
	

	indices = new int[getIndicesCount(width,height)];
	int i = 0;

	for (int row = 0; row<width - 1; row++) {
		if ((row & 1) == 0) { // even rows
			for (int col = 0; col<height; col++) {
				indices[i++] = col + row * height;
				indices[i++] = col + (row + 1) * height;
			}
		}
		else { // odd rows
			for (int col = height - 1; col>0; col--) {
				indices[i++] = col + (row + 1) * height;
				indices[i++] = col - 1 + +row * height;
			}
		}
	}
	if ((height & 1) && height > 2) {
		indices[i++] = (height - 1) * width;
	}

	return true;
}



void Grid::InitiateBuffers()
{
	GLuint vbo;
	GLuint ibo;

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, getVerticesCount(width, height) * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesCount(width, height) * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		

	shader = new Shader();
	shader->init(grid,grid);
}

void Grid::drawGrid()
{
	
	transformer.SetPos(glm::vec3(0, -20, -100));
	transformer.SetRot(this->rotation);
	transformer.SetScale(scale);
	
	

	if (shader)
	{
		shader->Bind();
		shader->Update(transformer, Camera::getSingleton());
	}

	

	//glDisable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glBindVertexArray(VAO);

	//glDrawArrays(GL_POINTS, 0, vertices.size());
	
	//glDrawArrays(GL_POINTS, 0, getVerticesCount(width, height));
	
	
	glDrawElements(GL_TRIANGLE_STRIP, getIndicesCount(width, height), GL_UNSIGNED_INT, 0);

	glCullFace(GL_BACK);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);


	

	
}



//float gridSizeX = 5;
//float gridSizeY = 5;
//float* gridVerts = NULL;
//float* ibData = NULL;
//int gridsize;
//int vertCount = 3;
//
//
//

//
//void Grid::GridMesh()
//{
//	
//
//	gridsize = (gridSizeX * gridSizeY);
//	gridVerts = new float[gridsize];
//
//	ibData = new float[gridSizeX * gridSizeY];
//
//	int i = 0;
//	int offset = 0;
//
//	//for (float z = 0; z < gridSizeY; z++)
//	//{
//	//	for (float x = 0; x < gridSizeX; x++)
//	//	{
//
//	//		Vert vert;
//
//	//		vert.x = x;
//	//		vert.y = 0;
//	//		vert.z = z + 1;
//
//	//		vertices.push_back(vert);			
//
//	//		vert.x = x;
//	//		vert.y = 0;
//	//		vert.z = z;
//
//	//		vertices.push_back(vert);
//
//
//	//		vert.x = x + 1;
//	//		vert.y = 0;
//	//		vert.z = z + 1;
//
//	//		vertices.push_back(vert);
//
//	//		vert.x = x + 1;
//	//		vert.y = 0;
//	//		vert.z = z;
//
//
//	//		vertices.push_back(vert);
//
//	//		i += 4;
//
//	//	}
//	//}
//
//	for (float z = 0; z < gridSizeY; z++)
//	{
//		for (float x = 0; x < gridSizeX; x++)
//		{
//
//			Vert vert;
//
//			vert.x = x;
//			vert.y = 0;
//			vert.z = z;
//
//			vertices.push_back(vert);
//
//		}
//	}
//	
//	for (size_t in = 0; in <  19 ; in++)
//	{
//		float indices1 = in;
//
//		indices.push_back(indices1);
//
//	}
//
//
//
//	GLuint vbo;
//	GLuint ibo;
//
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ibo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
//
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	
//	
//	glBindVertexArray(0);
//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	//gridVerts = 0;
//	//ibData = 0;
//
//	shader = new Shader();
//	shader->init(grid);
//
//	
//}
//
//void generateIBdata()
//{
//	
//
//}


