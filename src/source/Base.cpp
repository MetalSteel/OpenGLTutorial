#include <iostream>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

// GLFW窗口
GLFWwindow *window = nullptr;
// 窗口宽度
int width = 800;
// 窗口高度
int height = 600;
// 窗口标题
const char *title = "OpenGLTutorial";

// 摄像机
Camera camera;
// 帧时间
float deltaTime = 0.0f;
float lastTime = 0.0f;

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// 鼠标位置改变回调函数
void mouseCallback(GLFWwindow* window, double x, double y);
// 鼠标滑轮滚动回调事件
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
// 按键处理函数
void processInput(GLFWwindow *window);

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

    // 设置鼠标沉浸在窗口中
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 设置窗口大小改变回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    // 设置鼠标位置改变回调函数
    glfwSetCursorPosCallback(window, mouseCallback);
    // 设置鼠标滑轮滚动回调函数
    glfwSetScrollCallback(window, scrollCallback);

    // 初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Init Fail..." << std::endl;
        return EXIT_FAILURE;
    }

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 创建着色器
    Shader firstShader("../shader/VertexShaderSource.glsl", "../shader/FragmentShaderSource.glsl");

    // 顶点坐标数据
    GLfloat vertices[] =
    {
        // 顶点位置           // 贴图UV
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLfloat cubeVertices[] =
    {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    GLuint VAOs[1];
    glGenVertexArrays(1, VAOs);
    glBindVertexArray(VAOs[0]);

    GLuint VBOs[1];
    glGenBuffers(1, VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
    unsigned char *wallTexData = stbi_load("../texture/box01.png", &wallTexWidth, &wallTexHeight, &wallTexChannel, 0);
    // 将本地图片资源导入到贴图中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wallTexWidth, wallTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, wallTexData);
    // 生成MipMap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放图片资源
    stbi_image_free(wallTexData);

    // 激活着色器
    firstShader.use();
    // 设置Shader的纹理采样器的纹理单元
    firstShader.setUniform1i("boxTextureSampler", 0);

    // ############################################################
    GLuint CubeVAOs[1];
    glGenVertexArrays(1, CubeVAOs);
    glBindVertexArray(CubeVAOs[0]);

    GLuint CubeVBOs[1];
    glGenBuffers(1, CubeVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GL_FLOAT), (void*)0);

    // 渲染循环线程
    while(!glfwWindowShouldClose(window))
    {
        // 获取当前时间
        float currentTime = (float)glfwGetTime();
        // 获取差值时间
        deltaTime = currentTime - lastTime;
        // 设置最后时间
        lastTime = currentTime;

        // 处理键盘输入
        processInput(window);

        // 设置清除颜色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // 根据清除颜色清除颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 使用着色器程序
        firstShader.use();
        // 激活0纹理单元贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        // 模型矩阵
        glm::mat4 model(1.0f);
        // 视图矩阵
        glm::mat4 view;
        // 投影矩阵
        glm::mat4 projection(1.0f);

        // 模型矩阵,让模型围着一个轴旋转
        model = glm::rotate(model, (float)(glfwGetTime()*0.5), glm::vec3(0.5f, 1.0f, 0.0f));
        // 视图矩阵即摄像机
        view = camera.getViewMatrix();
        // 投影矩阵
        projection = glm::perspective(glm::radians(camera.getCameraFOV()), (float)(width/height), 0.1f, 100.0f);

        // 设置到顶点着色器
        firstShader.setUniformMatrix4fv("model", model);
        firstShader.setUniformMatrix4fv("view", view);
        firstShader.setUniformMatrix4fv("projection", projection);

        // 绑定顶点数组对象
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 双缓冲交换
        glfwSwapBuffers(window);
        // 轮询事件处理
        glfwPollEvents();
    }

    // 销毁顶点缓冲
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);

    // 销毁窗口并释放GLFW资源
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 鼠标位置改变回调函数
void mouseCallback(GLFWwindow *window, double x, double y)
{
    camera.processMouseInput(x, y);
}

// 鼠标滑轮滚动回调事件
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processScrollInput(yoffset);
}

// 键盘按键触发回调函数
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Forward, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Backward, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Right, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Left, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
