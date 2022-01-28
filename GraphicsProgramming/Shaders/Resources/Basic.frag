#version 450

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec2 FragTextureCoord;
in vec3 FragColorIn;
in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;


out vec4 frag_colour;


void main()
{
	

	//ambient
	float ambientStrength=1f;

	vec3 ambient=ambientStrength*FragLightColor;

	
	//diffuse
	//vec3 fragnormal=normalize(FragNormal);
	vec3 normal=normalize(texture2D(texture_normal,FragTextureCoord).rgb);
	normal=normalize(normal*2.0-1.0);
	normal=normalize(TBN*normal);
	//normal *= fragnormal;
	vec3 lightDir=normalize(FragLightPos-FragPos);
	float diff=(max(dot(normal,lightDir),0.0f));
	vec3 diffuse=diff*FragLightColor;

	//specular
	float specularStrength=1f;
	vec3 viewDir=normalize( FragCamPos - FragPos);
	vec3 reflectDir=reflect(lightDir,viewDir);
	float spec=pow(max(dot(reflectDir,normal),0.0),32);
	vec3 specular = vec3(specularStrength*spec);

	vec4 result=vec4(texture2D(texture_diffuse,FragTextureCoord).rgb*(ambient+diffuse+specular),1);
	//vec4 result = vec4(vec3(FragTextureCoord,0), 1);
	frag_colour=result;

};