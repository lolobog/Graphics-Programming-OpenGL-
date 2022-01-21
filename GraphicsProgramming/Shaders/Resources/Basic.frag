#version 450

uniform sampler2D texture_diffuse;


in vec2 FragTextureCoord;
in vec3 FragColorIn;
out vec4 frag_colour;


void main()
{
	//frag_colour = vec4(FragColorIn,1.0f);	

	frag_colour=vec4(texture2D(texture_diffuse,FragTextureCoord).rgb,1);
};