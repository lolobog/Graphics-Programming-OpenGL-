#pragma once

#include <glew.h>
#include"transform.h"
#include "Vertex.h"

enum {
	POSITION_VB,
	INDEX_VB,
	NUM_BUFFERS
};

class Mesh
{
public:
	Mesh(Vertex* verts, unsigned int vertCount,unsigned int *indices,unsigned int numIndicies);
	void Draw();
	~Mesh();

	Transform m_transform;
	unsigned int m_vertCount;
private:
	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;
	
};



