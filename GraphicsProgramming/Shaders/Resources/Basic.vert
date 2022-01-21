#version 450

in vec3 VectorPosition;
in vec2 TextureCoord;
in vec3 Color;
uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;


out vec2 FragTextureCoord;
out vec3 FragColorIn;
void main()
{
		FragColorIn=Color;
		FragTextureCoord=TextureCoord;
		gl_Position= perspective * view * model *vec4(VectorPosition,1.0) ; 
};