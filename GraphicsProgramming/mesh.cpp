#include "mesh.h"
#include "gtx/normal.hpp"

void Mesh::CalculateTangentsBitangent(Vertex* verticies, unsigned int vertCount, unsigned int* indicies, unsigned int numIndicies)
{
	//calculate tangent and bitangent
	for (unsigned int i = 0; i < numIndicies; i += 3)
	{
		//grab vertex for the current indece

		Vertex v0 = verticies[indicies[i]];
		Vertex v1 = verticies[indicies[i + 1]];
		Vertex v2 = verticies[indicies[i + 2]];

		vec3 edge1 = v1.Position - v0.Position;
		vec3 edge2 = v2.Position - v0.Position;

		GLfloat deltaU1 = v1.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV1 = v1.TextureCoord.y - v0.TextureCoord.y;
		GLfloat deltaU2 = v2.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV2 = v2.TextureCoord.y - v0.TextureCoord.y;

		GLfloat f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		vec3 tangent;
		vec3 biTangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		biTangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
		biTangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
		biTangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

		v0.Tangent += tangent;
		v1.Tangent += tangent;
		v2.Tangent += tangent;

		v0.Bitangent += biTangent;
		v1.Bitangent += biTangent;
		v2.Bitangent += biTangent;

		verticies[indicies[i]] = v0;
		verticies[indicies[i + 1]] = v1;
		verticies[indicies[i + 2]] = v2;
	}

	for (unsigned int i = 0; i < vertCount; i++)
	{
		verticies[i].Tangent = normalize(verticies[i].Tangent);
		verticies[i].Bitangent = normalize(verticies[i].Bitangent);
	}
}


Mesh::Mesh(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndicies)
{
	CalculateTangentsBitangent(verts, vertCount, indices, numIndicies);
	m_vertCount=numIndicies;

	std::vector <vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> colorInfo;
	//std::vector<vec3>Tangents;
	//std::vector<vec3>BiTangents;

	for (unsigned int i = 0; i < vertCount; i++)
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
		colorInfo.push_back(verts[i].Color);
		Tangents.push_back(verts[i].Tangent);
		BiTangents.push_back(verts[i].Bitangent);
		
	}
	
	//std::vector<vec3> Normals;
	Normals.resize(vertCount);


	for (int i = 0; i < vertCount; i++)
	{
		if (verts[i].Normal == vec3(0, 0, 0))
		{
			for (unsigned int i = 0; i < numIndicies; i += 3)
			{
				vec3 Vert1 = positions[indices[i]];
				vec3 Vert2 = positions[indices[i + 1]];
				vec3 Vert3 = positions[indices[i + 2]];

				vec3 normal = triangleNormal(Vert1, Vert2, Vert3);
				Normals[indices[i]] += normal;
				Normals[indices[i + 1]] += normal;
				Normals[indices[i + 2]] += normal;
			}
		}
		else
		{
			for (int i = 0; i < vertCount; i++)
				Normals.push_back(verts[i].Normal);
		}
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_vertexBufferObjects);

	//position

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(POSITION_VB);

	//Texture coords
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXCOORD_VB);

	//Color info
	/*glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(colorInfo[0]), &colorInfo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(COLOR_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOR_VB);*/

	//Normal calculations
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(NORMAL_VB);

	//Tangent info
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Tangents[0]), &Tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TANGENT_VB);

	//BiTangent info
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[BITANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(BiTangents[0]), &BiTangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(BITANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(BITANGENT_VB);

	//index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObjects[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_vertCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

