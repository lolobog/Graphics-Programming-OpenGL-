#pragma once
#include "glm.hpp"
struct Vertex
{ 
	vec3 Position;
	vec2 TextureCoord;
	vec3 Color;
	vec3 Tangent;
	vec3 Bitangent;

public:
	Vertex(float X, float Y, float Z)
	{
		this->Position.x = X;
		this->Position.y = Y;
		this->Position.z = Z;

		TextureCoord = { 0,0 };
		Color = { 0,0,0 };

		Tangent = vec3(0);
		Bitangent = vec3(0);
		
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
	
	Vertex(vec3 position) :Vertex(position.x, position.y, position.z)
	{

	}

	Vertex(vec3 position, vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
	}

	Vertex(vec3 position, vec2 texCoord,vec3 color)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
		this->Color = color;
	}
};