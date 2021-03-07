#version 430 core

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform vec3 lightColor;
out vec4 FragColor;

void main()
{   
    FragColor =  vec4(light.specular, 1.0f);
}