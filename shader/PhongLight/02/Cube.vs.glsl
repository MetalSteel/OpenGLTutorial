#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 worldVertexPosition;
out vec3 worldVertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
    worldVertexPosition = vec3(model * vec4(vertexPosition, 1.0f));
    worldVertexNormal = mat3(transpose(inverse(model))) * vertexNormal;
}
