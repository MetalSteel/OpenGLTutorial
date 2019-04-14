#ifndef OPENGLTUTORIAL_SHADER_H
#define OPENGLTUTORIAL_SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// 着色器类型枚举类
enum class ShaderType
{
    VertexShader, FragmentShader, ShaderProgram
};

// Shader工具类
class Shader
{
private:
    // 着色器程序id
    GLuint id;
public:
    // 着色器构造方法
    Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    // 着色器使用方法
    void use();
    // 设置Uniform变量整数类型
    void setUniform1i(const std::string &name, int value);
    // 设置Uniform变量浮点数类型
    void setUniform1f(const std::string &name, int value);
    // 设置Uniform变量齐次矩阵类型
    void setUniformMatrix4fv(const std::string &name, GLsizei count, GLboolean transpose, glm::mat4 value);
private:
    // 读取着色器文件
    std::string readShaderFile(const std::string &path);
    // 着色器检查
    void checkShader(GLuint id, ShaderType type);
};

#endif //OPENGLTUTORIAL_SHADER_H
