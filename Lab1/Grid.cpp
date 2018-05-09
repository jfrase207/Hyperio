#include "Grid.h"



Transform transformer;
int width = 20;
int height = 20;
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
	//GridMesh();
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

	for (int row = 0; row<width; row++) {
		for (int col = 0; col<height; col++) {
			

			vertices[i++] = (float)row;
			vertices[i++] = (rand() % 10 + 1) / 10;
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
				indices[i++] = col + row * width;
				indices[i++] = col + (row + 1) * width;
			}
		}
		else { // odd rows
			for (int col = width - 1; col>0; col--) {
				indices[i++] = col + (row + 1) * width;
				indices[i++] = col - 1 + +row * width;
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
	glBufferData(GL_ARRAY_BUFFER, getVerticesCount(10, 10) * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesCount(10, 10) * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);


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
	shader->init(grid);
}

void Grid::render() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLE_STRIP, getIndicesCount(width, height), GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glBindVertexArray(VAO);

	//glDrawArrays(GL_POINTS, 0, vertices.size());
	
	//glDrawArrays(GL_POINTS, 0, getVerticesCount(width, height));
	
	
	glDrawElements(GL_TRIANGLE_STRIP, getIndicesCount(width, height), GL_UNSIGNED_INT, 0);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);


	//glCullFace(GL_BACK);

	
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

