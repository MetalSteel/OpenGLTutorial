#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

// GLFW窗口
GLFWwindow *window = nullptr;
// 窗口宽度
int width = 800;
// 窗口高度
int height = 600;
// 窗口标题
const char *title = "OpenGLTutorial";

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// 键盘按键触发回调函数
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// 读取shader文件(**注意设置IDE编码为UTF-8)
std::string readShaderFile(const std::string &path);


// 主函数
int main(int argc, char **argv)
{
    // 初始化GLFW
    glfwInit();
    // 设置GLFW上下文版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 设置GLFW的版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建GLFW窗口
    window = glfwCreateWindow(width , height, title, nullptr, nullptr);
    // 判断窗口是否创建成功
    if(!window)
    {
        glfwTerminate();
        std::cout << "GLFW Window Create Fail..." << std::endl;
        return EXIT_FAILURE;
    }

    // 设置GLFW上下文为当前窗口线程上下文
    glfwMakeContextCurrent(window);
    // 设置窗口按键回调函数
    glfwSetKeyCallback(window, keyboardCallback);
    // 设置窗口大小改变回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // 初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Init Fail..." << std::endl;
        return EXIT_FAILURE;
    }

    // 顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderSource = readShaderFile("../shader/VertexShaderSource.vs").c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // 片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSource = readShaderFile("../shader/FragmentShaderSource.fs").c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // 着色器程序
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
    }

    // 删除顶点着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 顶点坐标数据
    GLfloat vertices[] =
    {
        // 顶点位置           // 顶点颜色           // 贴图UV
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    2.0f, 2.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    2.0f, -1.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   -1.0f, -1.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   -1.0f, 2.0f
    };
    // 顶点索引数据
    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VAOs[1];
    glGenVertexArrays(1, VAOs);
    glBindVertexArray(VAOs[0]);

    GLuint VBOs[1];
    glGenBuffers(1, VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBOs[1];
    glGenBuffers(1, EBOs);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // 加载创建贴图
    GLuint textures[1];
    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    // 设置贴图纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置贴图被缩小时,多级渐远纹理(Mipmap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // 设置贴图被放大时,像素之间线性插值,使得贴图更柔和
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 读取本地图片
    int wallTexWidth, wallTexHeight, wallTexChannel;
    unsigned char *wallTexData = stbi_load("../texture/wall.jpg", &wallTexWidth, &wallTexHeight, &wallTexChannel, 0);
    // 将本地图片资源导入到贴图中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallTexWidth, wallTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, wallTexData);
    // 生成MipMap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放图片资源
    stbi_image_free(wallTexData);

    // 激活着色器
    glUseProgram(program);
    // 设置Shader的纹理采样器的纹理单元
    glUniform1i(glGetUniformLocation(program, "wallTextureSampler"), 0);

    // 渲染循环线程
    while(!glfwWindowShouldClose(window))
    {
        // 设置清除颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 根据清除颜色清除颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 双缓冲交换
        glfwSwapBuffers(window);
        // 轮询事件处理
        glfwPollEvents();
    }

    // 销毁顶点缓冲
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBOs);

    // 销毁窗口并释放GLFW资源
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    std::cout << width << " = " << height << std::endl;
    glViewport(0, 0, width, height);
}

// 键盘按键触发回调函数
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// 读取shader文件(**注意设置IDE编码为UTF-8)
std::string readShaderFile(const std::string &path)
{
    // 打开shader文件
    std::ifstream ifile(path);
    // 返回结果
    std::string result;
    // 循环读取
    while(ifile.good())
    {
        // 创建一行
        std::string line;
        // 读取一行
        std::getline(ifile, line);
        // 注意:不要忘记加上换行
        result.append(line + "\n");
    }

    return result;
}