#version 330 core

// 顶点位置
layout(location = 0) in vec3 vertexPosition;
// 顶点法线
layout(location = 1) in vec3 vertexNormal;
// 顶点UV
layout(location = 2) in vec2 vertexUVIn;

// 输出世界坐标系_顶点位置
out vec3 worldVertexPosition;
// 输出世界坐标系_顶点法线
out vec3 worldVertexNormal;
// 输出顶点UV
out vec2 vertexUV;

// 模型矩阵
uniform mat4 model;
// 视图矩阵
uniform mat4 view;
// 裁剪矩阵
uniform mat4 projection;

void main()
{
    // 输出顶点位置
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
    // 输出世界坐标系的顶点位置
    worldVertexPosition = vec3(model * vec4(vertexPosition, 1.0f));
    // 输出世界坐标系的法线
    worldVertexNormal = mat3(transpose(inverse(model))) * vertexNormal;
    // 输出顶点UV
    vertexUV = vertexUVIn;
}
