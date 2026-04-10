#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;

uniform Light light;
uniform Light sunLight;
uniform Light moonLight;

uniform sampler2D texture_diffusse;

vec3 CalcLight(Light currentLight, vec3 norm, vec3 viewDir)
{
    vec3 ambient = currentLight.ambient * material.ambient;

    vec3 lightDir = normalize(currentLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = currentLight.diffuse * diff * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = currentLight.specular * spec * material.specular;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    result += CalcLight(light, norm, viewDir);
    result += CalcLight(sunLight, norm, viewDir);
    result += CalcLight(moonLight, norm, viewDir);

    color = vec4(result, 1.0f) * texture(texture_diffusse, TexCoords);
}