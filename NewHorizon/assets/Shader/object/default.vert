#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
out vec2 TexCoords;
uniform mat4 modelViewProjection;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = modelViewProjection * vec4(aPos, 1.0);//projection * view * model
}