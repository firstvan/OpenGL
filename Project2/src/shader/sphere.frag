#version 430
struct Material {
    float shininess;
}; 

layout (binding = 0) uniform sampler2D Tex1;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

layout( location = 0 ) out vec4 FragColor;

uniform vec3 viewPos;
uniform PointLight pointLight;
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec ;
    return (ambient + diffuse + specular);
}

void main()
{    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
	vec4 texC = texture(Tex1, TexCoords);
	FragColor = vec4(CalcPointLight(pointLight, norm, FragPos, viewDir), 1.0f) * texC;
	
}

