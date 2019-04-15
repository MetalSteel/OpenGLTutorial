#include <iostream>
#include "Camera.h"
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

// 窗口标题
const char *title = "PhongLight";
// 窗口宽度和高度
int width = 800, height = 600;
// 窗口对象
GLFWwindow *window = nullptr;

// 帧差时间
float deltaTime = 0.0f;
// 上一帧时间
float lastTime = 0.0f;

// 摄像机
Camera camera;

// 光物体位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// 鼠标位置改变回调函数
void cursorPosCallback(GLFWwindow *window, double x, double y);
// 鼠标滚轮改变回调函数
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
// 键盘输入回调函数
void keyboardInput(GLFWwindow *window);

// 获取OpenGL信息
void getDeviceGLInfo();

int main()
{
    // 初始化GLFW
    glfwInit();
    // 设置GLFW窗口上下文的OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 设置GLFW窗口的OpenGL渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 创建GLFW窗口
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    // 判断是否创建成功
    if(!window)
    {
        glfwTerminate();
        std::cout << "GLFW Window Create Fail..." << std::endl;
        return EXIT_FAILURE;
    }
    // 设置GLFW上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Init Fail..." << std::endl;
        return EXIT_FAILURE;
    }

    // 获取OpenGL版本及设备信息
    getDeviceGLInfo();

    // 设置鼠标沉浸模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 设置窗口大小改变回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    // 设置鼠标输入回调函数
    glfwSetCursorPosCallback(window, cursorPosCallback);
    // 设置鼠标滚轮回调函数
    glfwSetScrollCallback(window, scrollCallback);

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 立方体顶点数据
    GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // 创建两个Shader
    Shader lightShader("../shader/PhongLight/02/Light.vs.glsl","../shader/PhongLight/02/Light.fs.glsl");
    Shader objShader("../shader/PhongLight/02/Cube.vs.glsl","../shader/PhongLight/02/Cube.fs.glsl");

    // 立方体物体VBO
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // 光照物体VAO
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 绑定立方体物体VBO
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // VAO解释VBO中的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0);
    // 启用VAO中的0号属性位置
    glEnableVertexAttribArray(0);

    // 立方体VAO
    GLuint objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);
    // 绑定立方体物体VBO
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // VAO解释VBO中的顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0);
    // 启用VAO中的0号顶点属性位置
    glEnableVertexAttribArray(0);
    // VAO解释VBO中的法线数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    // 启用VAO中的0号法线属性位置
    glEnableVertexAttribArray(1);

    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 计算时间帧差
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 设置颜色缓冲区清除颜色
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // 清除颜色缓冲区与深度缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 键盘输入
        keyboardInput(window);

        // 视图矩阵
        glm::mat4 view = camera.getViewMatrix();
        // 裁剪矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.getCameraFOV()), (float)width/(float)height, 0.1f, 100.0f);

        // 设置光物体着色器
        lightShader.use();

        // 设置光物体顶点着色器模型矩阵
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2));
        lightShader.setUniformMatrix4fv("model", lightModel);
        // 设置光物体顶点着色器视图矩阵
        lightShader.setUniformMatrix4fv("view", view);
        // 设置光物体顶点着色器裁剪矩阵
        lightShader.setUniformMatrix4fv("projection", projection);

        // 绘制光物体
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 设置立方体物体着色器
        objShader.use();

        // 设置立方体物体顶点着色器模型矩阵、视图矩阵、裁剪矩阵
        glm::mat4 objModel(1.0f);
        objModel = glm::rotate(objModel, (float)(glfwGetTime()*0.5), glm::vec3(0.5f, 1.0f, 0.0f));
        objShader.setUniformMatrix4fv("model", objModel);
        objShader.setUniformMatrix4fv("view", view);
        objShader.setUniformMatrix4fv("projection", projection);

        // 相机位置
        objShader.setUniform3fv("cameraPosition", camera.getCameraPosition());

        // 材质属性由本身的材质特点决定
        objShader.setUniform3fv("material.ambient", glm::vec3(0.0215f, 0.1745f, 0.0215f));
        objShader.setUniform3fv("material.diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
        objShader.setUniform3fv("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
        objShader.setUniform1f("material.shininess", 0.6f*128);

        // 光源分解为3个分量,环境光一般较弱,漫反射光源一般为光实际的颜色,镜面光一般设置为最大(白色)
        objShader.setUniform3fv("light.ambient", glm::vec3(0.1f));
        objShader.setUniform3fv("light.diffuse", glm::vec3(0.8f));
        objShader.setUniform3fv("light.specular", glm::vec3(1.0f));
        objShader.setUniform3fv("light.position", lightPos);

        // 绘制立方体物体
        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 双缓冲交换
        glfwSwapBuffers(window);
        // 事件处理
        glfwPollEvents();
    }

    // 关闭窗口释放资源
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
void cursorPosCallback(GLFWwindow *window, double x, double y)
{
    camera.processMouseInput(x, y);
}

// 鼠标滚轮改变回调函数
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processScrollInput(yoffset);
}

// 键盘输入回调函数
void keyboardInput(GLFWwindow *window)
{
    // 关闭窗口
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // 向前移动
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Forward, deltaTime);
    }
    // 向后移动
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Backward, deltaTime);
    }
    // 向右移动
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Right, deltaTime);
    }
    // 向左移动
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyInput(CameraMovement::Left, deltaTime);
    }
}

// 获取OpenGL信息
void getDeviceGLInfo()
{
    // OpenGL实现厂商名字
    const GLubyte *vendor = glGetString(GL_VENDOR);
    // 渲染器名字,通常是一个硬件平台
    const GLubyte *renderer = glGetString(GL_RENDERER);
    // 当前OpenGL实现的版本号
    const GLubyte *version = glGetString(GL_VERSION);

    std::cout << "vendor = " << vendor << ",renderer = " << renderer << ",version = " << version << std::endl;
}
