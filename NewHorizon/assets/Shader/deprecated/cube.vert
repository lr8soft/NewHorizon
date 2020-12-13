#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    mat3 NormalMatrix =  mat3(transpose(inverse(model)));

    TexCoords = aTexCoords;    
    FragNormal = NormalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));
}