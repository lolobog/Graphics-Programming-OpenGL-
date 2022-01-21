#version 450

in vec3 VectorPosition;
in vec2 TextureCoord;
in vec3 Color;
in vec3 Normal;

uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;


out vec2 FragTextureCoord;
out vec3 FragColorIn;
out vec3 FragNormal;
out vec3 FragPos;
void main()
{
FragPos=vec3(model*vec4(VectorPosition,1.0f));
FragNormal=mat3(transpose(inverse(model)))*Normal;
		FragColorIn=Color;
		FragTextureCoord=TextureCoord;
		gl_Position= perspective * view * model *vec4(VectorPosition,1.0) ; 
};