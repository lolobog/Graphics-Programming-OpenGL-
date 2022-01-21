#pragma once
#include "glm.hpp"
struct Vertex
{ 
	vec3 Position;
	vec2 TextureCoord;
	vec3 Color;

public:
	Vertex(float X, float Y, float Z)
	{
		this->Position.x = X;
		this->Position.y = Y;
		this->Position.z = Z;

		TextureCoord = { 0,0 };
		Color = { 0,0,0 };
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