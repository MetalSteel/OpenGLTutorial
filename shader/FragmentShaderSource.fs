#version 330 core

in vec2 outTexUV;
out vec4 finalColor;

uniform sampler2D boxTextureSampler;

void main()
{
    finalColor = texture(boxTextureSampler, outTexUV);
}