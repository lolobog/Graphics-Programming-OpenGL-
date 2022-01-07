#include "mesh.h"

Mesh::Mesh(float* verts, unsigned int vertCount)
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
