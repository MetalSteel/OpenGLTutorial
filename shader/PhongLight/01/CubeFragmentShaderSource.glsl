#version 330 core

out vec4 finalColor;
in vec3 fNormal;
in vec3 worldVertexPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - worldVertexPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float speclarStrength = 0.5;
    vec3 viewDir = normalize(viewPos - worldVertexPos);
    vec3 lightReflect = reflect(-lightDir, normal);
    float spec = pow(max(dot(lightReflect, viewDir), 0.0), 128);
    vec3 specular = speclarStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;

    finalColor = vec4(result, 1.0f);
}