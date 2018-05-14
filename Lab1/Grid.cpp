#include "Grid.h"
#include "PerlinNoise.h"



Transform transformer;
int width = 200;
int height = 200;
glm::vec3 scale = glm::vec3(5, 1, 5);
float* vertices = 0;
int* indices = 0;
float seed = 0;
float offset = 0.2;

Grid::Grid()
{

}

Grid::~Grid()
{

}

int getVerticesCount(int width, int height) {
	return width * height * 3;
}

int getIndicesCount(int width, int height) {
	return (width*height) + (width - 1)*(height - 2);
}


bool Grid::init()
{
	
	vertices = new float[getVerticesCount(width, height)];
	indices = new int[getIndicesCount(width, height)];

	getVertices(width,height);
	getIndices(width,height);
	InitiateBuffers();
	
	return true;
}

bool Grid::getVertices(int width, int height) 
{
	
	int i = 0;

	srand(time(NULL));
	seed = rand() % 1000 + 1;
	

	//PerlinNoise pn(offset+=0.4);
	PerlinNoise pn(seed);

	for (int row = 0; row<width; row++) {
		for (int col = 0; col<height; col++) {

			double x = (double)row / ((double)width);
			double y = (double)col / ((double)height);

			float n = pn.noise(10 * x, 0.8, 10 * y);
			float yVer = n * 30;

			vertices[i++] = (float)row;
			vertices[i++] = yVer;
			vertices[i++] = (float)col;
		}
	}

	return true;
}



bool Grid::getIndices(int width, int height) 
{	
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		

	shader = new Shader();
	shader->init(grid,grid);
}

void Grid::drawGrid()
{
	/*getVertices(width, height);
	getIndices(width, height);
	InitiateBuffers();
*/
	transformer.SetPos(glm::vec3(-200, -20, -200));
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


