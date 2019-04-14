#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 inNormal;

out vec3 fNormal;
out vec3 worldVertexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPos, 1.0f);
    // 将顶点的法线输出到片段着色器
    // 这里要将顶点的法线也转换成世界坐标系中
    // 如果物体缩放/旋转,则原来的法线就不适用了
    // 因此法线也必须要转换到世界坐标系中
    fNormal = mat3(transpose(inverse(model))) * inNormal;
    // 将顶点的世界位置发送到片段着色器
    worldVertexPos = vec3(model * vec4(vertexPos, 1.0f));
}