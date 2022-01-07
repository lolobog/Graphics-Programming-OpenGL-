#pragma once

#include <glew.h>
#include"transform.h"

class Mesh
{
public:
	Mesh(float* verts, unsigned int vertCount);
	void Draw();
	~Mesh();

	Transform m_transform;

private:
	GLuint m_vertexBufferObject = 0;
	GLuint m_vertexArrayObject = 0;
};

