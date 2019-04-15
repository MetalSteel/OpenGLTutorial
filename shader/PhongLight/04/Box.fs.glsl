#version 330 core
in vec3 worldVertexPosition;
in vec3 worldVertexNormal;
in vec2 vertexUV;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirectionLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material material;
uniform PointLight light;
uniform vec3 cameraPosition;
out vec4 finalColor;
void main()
{
    float distance = length(worldVertexPosition - light.position);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    vec3 ambient = light.ambient * texture(material.diffuse, vertexUV).rgb;
    ambient *= attenuation;
    vec3 normal = normalize(worldVertexNormal);
    vec3 lightDirInv = normalize(light.position - worldVertexPosition);
    float diff = max(dot(lightDirInv, normal), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, vertexUV).rgb;
    diffuse *= attenuation;
    vec3 lightReflect = normalize(reflect(-lightDirInv, normal));
    vec3 viewDirRef = normalize(cameraPosition - worldVertexPosition);
    float spec = pow(max(dot(lightReflect, viewDirRef), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, vertexUV).rgb;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    finalColor = vec4(result, 1.0f);
}
