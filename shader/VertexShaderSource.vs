#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 texUV;

out vec3 outVertexColor;
out vec2 outTexUV;

void main()
{
    gl_Position = vec4(vertexPos, 1.0f);
    outVertexColor = vertexColor;
    outTexUV = texUV;
}