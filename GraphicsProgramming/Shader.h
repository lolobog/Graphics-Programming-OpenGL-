#pragma once
#include <glew.h>
#include <string>
#include <fstream>
#include "transform.h"
#include "Camera.h"
#include <iostream>
#include "LightBase.h"

using namespace std;

class Shader
{
public:
	Shader(const string FileLocation, Camera& camera);
	~Shader();

	void Update(Transform& transform,LightBase& light);
	void Bind();
	GLuint GetProgram() { return m_program; }


	void UpdateForShadow(Transform& transform, glm::mat4& LightSpaceMatrix);
	


private:
	string Name;
	GLuint m_program;
	Camera* m_camera;
	enum ShaderTypes
	{
		VERTEX,
		FRAGMENT,
		NUM_SHADER_TYPES
	};
	GLuint m_shaders[NUM_SHADER_TYPES];

	enum UniformNames
	{
		MODEL_U,
		PROJECTION_U,
		VIEW_U,
		FRAG_LIGHTCOLOR_U,
		FRAG_LIGHTPOS_U,
		FRAG_CAMERAPOS_U,
		NUM_UNIFORMS
	};
	GLuint m_uniforms[NUM_UNIFORMS];
};

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		cerr << errorMessage << ": '" << error << "'" << endl;

	}

	
		
}

