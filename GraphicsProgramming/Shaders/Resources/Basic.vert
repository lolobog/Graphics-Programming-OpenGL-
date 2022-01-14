#version 450

in vec3 vp;
uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;

void main()
{
		gl_Position= perspective * view * model *vec4(vp,1.0) ; 
};