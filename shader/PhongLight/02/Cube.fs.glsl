#version 330 core

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;

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

uniform vec3 cameraPosition;
uniform Material material;
uniform Light light;

out vec4 finalColor;

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 normal = normalize(worldVertexNormal);
    vec3 lightDirInv = normalize(light.position - worldVertexPosition);
    float cosAngle = max(dot(lightDirInv, normal), 0.0f);
    vec3 diffuse = light.diffuse * cosAngle * material.diffuse;

    vec3 viewDir = normalize(cameraPosition - worldVertexPosition);
    vec3 lightRefDir = normalize(reflect(-lightDirInv, normal));
    float spec = pow(max(dot(lightRefDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result = ambient + diffuse + specular;

    finalColor = vec4(result, 1.0f);
}