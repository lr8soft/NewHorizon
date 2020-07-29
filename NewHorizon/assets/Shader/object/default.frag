#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragNormal;
in vec3 FragPos;
uniform sampler2D texture_diffuse1;

uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{   
    float ambientRate = 0.2f;
    vec3 ambient = ambientRate * ambientColor;

    vec3 norm = normalize(FragNormal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse);
    vec4 resultVec4 = vec4(result, 1.0f) * texture(texture_diffuse1, TexCoords);

    FragColor =  resultVec4;

}