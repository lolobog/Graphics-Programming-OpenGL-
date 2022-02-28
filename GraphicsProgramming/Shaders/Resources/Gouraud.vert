#version 450

in vec3 VectorPosition;
in vec2 TextureCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;
uniform vec4 lightColor;

varying vec4 v_Color;

void main()
{
mat4 u_MVMatrix= model*view;
mat4 u_MVPMatrix=model*view*perspective;

// Transform the vertex into eye space.
    vec3 modelViewVertex = vec3(u_MVMatrix * vec4(VectorPosition, 0.0));
 
    // Transform the normal's orientation into eye space.
    vec3 modelViewNormal = vec3(u_MVMatrix * vec4(Normal, 0.0));
 
    // Will be used for attenuation.
    float distance = length(lightPos - modelViewVertex);
 
    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(lightPos - modelViewVertex);
 
    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(modelViewNormal, lightVector), 0.1);
 
    // Attenuate the light based on distance.
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
 
    // Multiply the color by the illumination level. It will be interpolated across the triangle.
    v_Color = lightColor * diffuse;
 
    // gl_Position is a special variable used to store the final position.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = u_MVPMatrix* vec4(VectorPosition, 0.0);

  }