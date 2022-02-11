#include "Shader.h"



Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}

void Shader::Update(Transform& transform,LightBase& light)
{
	mat4 projection = m_camera->GetPerspective();
	mat4 view = m_camera->CalculateViewMatrix();
	mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);

	glUniform3f(m_uniforms[FRAG_CAMERAPOS_U], m_camera->GetTransform().GetPosition().x,
		m_camera->GetTransform().GetPosition().y,
		m_camera->GetTransform().GetPosition().z);

	glUniform3f(m_uniforms[FRAG_LIGHTPOS_U], light.GetTransform().GetPosition().x,
		light.GetTransform().GetPosition().y,
		light.GetTransform().GetPosition().z);

	glUniform3f(m_uniforms[FRAG_LIGHTCOLOR_U], light.M_Color.x,
		light.M_Color.y,
		light.M_Color.z);

}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::UpdateForShadow(Transform& transform, glm::mat4& LightSpaceMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program,))
}

static GLuint CreateShader(const string& ShaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		cerr << "Error: shader creation failed" << std::endl;
	}

	const char* tempSourceCode = ShaderSource.c_str();

	glShaderSource(shader, 1, &tempSourceCode, NULL);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: shader compilation failed: ");
	return shader;	
}

static string LoadShader(const string& fileName)
{
	ifstream file;
	const char* fileNameChar = fileName.c_str();
	file.open(fileNameChar, ifstream::in);

	string output;
	string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		cerr << "Unable to load shader: " << fileName << endl;
	}
	return output;
}

Shader::Shader(const std::string FileLocation, Camera& camera)
{
	m_camera = &camera;
	Name = FileLocation;
	m_program = glCreateProgram();
	m_shaders[VERTEX] = CreateShader(LoadShader(FileLocation + ".vert"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT] = CreateShader(LoadShader(FileLocation + ".frag"), GL_FRAGMENT_SHADER);

	for (GLuint i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program Linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "model");
	m_uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "perspective");
	m_uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");

	m_uniforms[FRAG_CAMERAPOS_U] = glGetUniformLocation(m_program, "FragCamPos");
	m_uniforms[FRAG_LIGHTCOLOR_U] = glGetUniformLocation(m_program, "FragLightColor");
	m_uniforms[FRAG_LIGHTPOS_U] = glGetUniformLocation(m_program, "FragLightPos");


	for (GLuint i = 0; i < NUM_UNIFORMS; i++)
	{
		if (m_uniforms[i] == GL_INVALID_INDEX)
			cout << "Shader " << Name << " Uniform invalid index:" << static_cast<UniformNames>(i)
			<< "(Might be optimized out if not used)" << endl;

	}
}