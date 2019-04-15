#version 330 core
in vec3 worldVertexPosition;
in vec3 worldVertexNormal;
in vec2 vertexUV;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 cameraPosition;

out vec4 finalColor;

void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, vertexUV).rgb;

    vec3 normal = normalize(worldVertexNormal);
    vec3 lightDirInv = normalize(light.position - worldVertexPosition);
    float diff = max(dot(lightDirInv, normal), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, vertexUV).rgb;

    vec3 lightReflect = normalize(reflect(-lightDirInv, normal));
    vec3 viewDirRef = normalize(cameraPosition - worldVertexPosition);
    float spec = pow(max(dot(lightReflect, viewDirRef), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, vertexUV).rgb;

    vec3 emission = texture(material.emission, vertexUV).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    finalColor = vec4(result, 1.0f);
}
