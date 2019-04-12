#version 330 core

in vec3 outVertexColor;
in vec2 outTexUV;
out vec4 finalColor;

uniform sampler2D wallTextureSampler;

void main()
{
    finalColor = texture(wallTextureSampler, outTexUV) * vec4(outVertexColor, 1.0f);
}