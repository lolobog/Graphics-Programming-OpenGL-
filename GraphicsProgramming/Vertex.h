#pragma once
#include "glm.hpp"
struct Vertex
{ 
	glm::vec3 Position;
	glm::vec2 TextureCoord;
	glm::vec3 Color;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	glm::vec3 Normal= vec3(0,0,0);

public:
	Vertex(float X, float Y, float Z)
	{
		this->Position.x = X;
		this->Position.y = Y;
		this->Position.z = Z;

		TextureCoord = { 0,0 };
		Color = { 0,0,0 };

		Tangent = glm::vec3(0);
		Bitangent = glm::vec3(0);
		
	}

	Vertex(const Vertex& vert)
	{
			this->Position.x = vert.Position.x;
			this->Position.y = vert.Position.y;
			this->Position.z = vert.Position.z;

			TextureCoord = vert.TextureCoord;
			Color = vert.Color;

			Tangent = vert.Tangent;
			Bitangent = vert.Bitangent;
	}

	Vertex();
	
	Vertex(glm::vec3 position) :Vertex(position.x, position.y, position.z)
	{

	}

	Vertex(glm::vec3 position, glm::vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
	}

	Vertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 color)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
		this->Color = color;
	}
};