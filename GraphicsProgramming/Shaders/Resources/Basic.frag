#version 450

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_Shadow;
uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;


in vec2 FragTextureCoord;
in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;
in vec4 FragPosLightSpace;
in vec3 VsNormal;
out vec4 frag_colour;

float CalculateShadowValue()
{
	
	
	vec3 projCoords = FragPosLightSpace.xyz/ FragPosLightSpace.w;
	projCoords = projCoords * 0.5 +0.5;

	float closestDepth = texture(texture_Shadow, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.0005;


	vec3 lightDir = normalize(FragLightPos - FragPos);



	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(texture_Shadow, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(texture_Shadow, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;


	if(projCoords.z >1.0)
	{
		shadow = 0;
	}

	return shadow;
}

void main()
{


	//ambiant
	float ambientStrength =  1;
    vec3 ambient = ambientStrength *texture2D(texture_diffuse, FragTextureCoord).rgb ;


	vec3 normal = normalize( texture2D(texture_normal, FragTextureCoord).rgb);
	normal = normalize(normal * 2.0-1.0);
	normal = normalize(TBN*normal );

	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, -lightDir ), 0.0);
	vec3 diffuse = diff   *texture2D(texture_diffuse, FragTextureCoord).rgb; 
	diffuse*= 0.5;

	//Specular
	float specularStrength =  0.8f;
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(normal, reflectDir), 0.0), 16.0);
    vec3 specular = vec3(specularStrength * spec);
	



	float shadow=CalculateShadowValue();
	vec4 result=vec4( (ambient+(1.0-shadow))*(diffuse+specular),1); 
	frag_colour=result;

	

	
	

};