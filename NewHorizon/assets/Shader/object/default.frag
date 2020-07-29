#version 330 core

uniform sampler2D texture_diffuse1;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragNormal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{   
    float ambientRate = 0.1f;
    vec3 ambient = ambientRate * ambientColor;

    vec3 norm = normalize(FragNormal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec4 diffuseTex = texture(texture_diffuse1, TexCoords);
    vec3 diffuse = diff * vec3(diffuseTex.rgb) * lightColor;

    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 8);//shininess=16
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular);

    FragColor =  vec4(result, 1.0f);

}