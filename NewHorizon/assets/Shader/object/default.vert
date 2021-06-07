#version 430 core

#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 dirShadowMat[MAX_DIRECTIONAL_LIGHTS];
uniform mat4 pointShadowMat[MAX_POINT_LIGHTS];
uniform mat4 spotShadowMat[MAX_SPOT_LIGHTS];

uniform vec3 lightCount;

out vec2 TexCoords;
out vec3 FragNormal;
out vec3 FragPos;

out vec4 dirShadowLightSpace[MAX_DIRECTIONAL_LIGHTS];
out vec4 pointShadowLightSpace[MAX_POINT_LIGHTS];
out vec4 spotShadowLightSpace[MAX_SPOT_LIGHTS];

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    mat3 NormalMatrix =  mat3(transpose(inverse(model)));

    TexCoords = aTexCoords;    
    FragNormal = NormalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));

    for(int i = 0; i < lightCount.x; i++)
        dirShadowLightSpace[i] = dirShadowMat[i] * vec4(FragPos, 1.0);
}