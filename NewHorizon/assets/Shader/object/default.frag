#version 430 core

#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D shadow_map;
    float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoords;

in vec4 dirShadowLightSpace[MAX_DIRECTIONAL_LIGHTS];
in vec4 pointShadowLightSpace[MAX_POINT_LIGHTS];
in vec4 spotShadowLightSpace[MAX_SPOT_LIGHTS];

out vec4 FragColor;

uniform vec3 lightCount; //dir, point, spot

uniform vec3 viewPos;
uniform DirLight dirLight[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];



uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{    
    // properties
    vec3 norm = normalize(FragNormal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // phase 1: directional lighting
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < lightCount.x; i++)
    {
        float shadow = ShadowCalculation(dirShadowLightSpace[i]);
        result += CalcDirLight(dirLight[i], norm, viewDir, shadow);
    }




    // phase 2: point lights
    for(int j = 0; j < lightCount.y; j++)
        result += CalcPointLight(pointLights[j], norm, FragPos, viewDir);  
  
    // phase 3: spot light
    for(int k = 0; k < lightCount.z; k++)
        result += CalcSpotLight(spotLight[k], norm, FragPos, viewDir);   

 
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    //blinn-phong
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    float displayPercent = 1.0 - shadow;

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + displayPercent * (diffuse + specular) );
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // projection div
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transfor to [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    // get the closest depth
    float closestDepth = texture(material.shadow_map, projCoords.xy).r; 
    // get current fragment depth
    float currentDepth = projCoords.z;

    float offset = 0.0005f;

    if(projCoords.z > 1.0)  //force shadow = 0 when z > 1.0
        return 0.0;


    bool usePcf = true;
    float shadow = 0.0;
    if(usePcf)
    {
        vec2 texelSize = 1.0 / textureSize(material.shadow_map, 0);
        for(int x=-1; x<=1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(material.shadow_map, projCoords.xy + vec2(x,y) * texelSize ).r;
                shadow += currentDepth - offset > pcfDepth ? 1.0 : 0.0;
            }

        }
        shadow /= 9.0;
    }else{
        // current frag is in shadow?
        shadow = currentDepth - offset > closestDepth  ? 1.0 : 0.0;
    }


    return shadow;
}