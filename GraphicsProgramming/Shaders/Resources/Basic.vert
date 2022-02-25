#version 450

in vec3 VectorPosition;
in vec2 TextureCoord;
//in vec3 Color;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

out vec2 FragTextureCoord;
out vec3 FragColorIn;
out vec3 FragNormal;
out vec3 FragPos;
out mat3 TBN;
out vec4 FragPosLightSpace;
//out vec3 VsNormal;

void main()
{



		////
		FragPos=vec3(model*vec4(VectorPosition,1.0f));
		FragNormal=mat3(transpose(inverse(model)))*Normal;
		FragNormal=Normal;
		////
		FragPosLightSpace=lightSpaceMatrix*vec4(FragPos,1.0);
		//VsNormal = Normal;

		//FragColorIn=Color;
		FragTextureCoord=TextureCoord;
		gl_Position= perspective * view * model *vec4(VectorPosition,1.0) ; 

		vec3 T=normalize(vec3(model*vec4(Tangent,0.0)));
		vec3 B=normalize(vec3(model*vec4(BiTangent,0.0)));
		vec3 N=normalize(vec3(model*vec4(Normal,0.0)));

		TBN=mat3(T,B,N);
};