#version 430 core
const int MAX_POINT_LIGHT = 10;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform DirectionalLight light;
uniform vec3 viewPos; //current camera postion

in vec2 TexCoords;
in vec3 FragNormal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{   
    vec3 diffuseTexture = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specularTexture = vec3(texture(material.texture_specular1, TexCoords));
    vec3 ambient = light.ambient * diffuseTexture;

    vec3 norm = normalize(FragNormal);
    
    //vec3 lightDirection = normalize(light.position - FragPos);
    vec3 lightDirection = normalize(-light.direction);

    float diff = max(dot(norm, lightDirection), 0.0);
    
    vec3 diffuse = diff  * diffuseTexture * light.diffuse;

    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);//shininess=16
    vec3 specular = light.specular * spec * specularTexture;

    vec3 result = (ambient + diffuse + specular);

    FragColor =  vec4(result, 1.0f);

}