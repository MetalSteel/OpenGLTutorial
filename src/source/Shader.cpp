#include "Shader.h"

// 着色器构造方法
Shader::Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    // 顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string tmpStr = readShaderFile(vertexShaderSource);
    tmpStr.erase(0, 20);
    const char *vertexShaderCode = tmpStr.c_str();
    std::cout << "## " << vertexShaderSource << " ##" << std::endl;
    std::cout << vertexShaderCode << std::endl;
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);
    checkShader(vertexShader, ShaderType::VertexShader);

    // 片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderCode = readShaderFile(fragmentShaderSource).c_str();
    std::cout << "## " << fragmentShaderSource << " ##" << std::endl;
    std::cout << fragmentShaderCode << std::endl;
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader, ShaderType::FragmentShader);

    // 着色器程序
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    checkShader(id, ShaderType::ShaderProgram);

    // 删除顶点着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// 着色器使用方法
void Shader::use()
{
    glUseProgram(id);
}

// 读取着色器文件
std::string Shader::readShaderFile(const std::string &path)
{
    // 打开shader文件
    std::ifstream ifile(path, std::ios::binary);
    // 返回结果
    std::string tmp("         \n\0");
    std::string result;
    result.append(tmp);
    result.append("#version 330 core\n");
    // 判断释放打开成功
    if(!ifile.is_open())
    {
        result = "Read File Fail...";
        return result;
    }
    // 判断是否读取第一行
    bool bIsFirstLine = true;
    // 循环读取
    while(ifile)
    {
        // 创建一行
        std::string line;
        // 读取一行
        std::getline(ifile, line);
        // 判断是否是第一行,舍弃第一行
        if(bIsFirstLine)
        {
            bIsFirstLine = false;
            continue;
        }
        // 判断是否空行
        if(0 == line.size())
        {
            continue;
        }
        // 判断是否注释
        if(std::string::npos != line.find("//"))
        {
            continue;
        }
        // 注意:不要忘记加上换行
        result.append(line + "\n");
    }
    result.append("\0");
    std::cout << result << std::endl;

    ifile.close();

    return result;
}

// 着色器检查
void Shader::checkShader(GLuint id, ShaderType type)
{
    int success;
    char infoLog[512];

    switch(type)
    {
        case ShaderType::VertexShader:
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
            }
            break;
        case ShaderType::FragmentShader:
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
            }
            break;
        case ShaderType::ShaderProgram:
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(id, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
            }
            break;
        default:
            break;
    }
}

// 设置Uniform变量整数类型
void Shader::setUniform1i(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

// 设置Uniform变量浮点数类型
void Shader::setUniform1f(const std::string &name, int value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

// 设置Uniform变量vec3类型
void Shader::setUniform3fv(const std::string &name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

// 设置Uniform变量齐次矩阵类型
void Shader::setUniformMatrix4fv(const std::string &name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


