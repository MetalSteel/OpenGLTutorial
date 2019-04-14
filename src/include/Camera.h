#ifndef OPENGLTUTORIAL_CAMERA_H
#define OPENGLTUTORIAL_CAMERA_H

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// 摄像机移动方向
enum class CameraMovement
{
    Forward,Backward,Left,Right
};

class Camera
{
private:
    // 摄像机位置
    glm::vec3 cameraPosition;
    // 世界坐标上向量
    glm::vec3 worldUp;

    // 摄像机欧拉角俯仰角(角度)
    float cameraPitch;
    // 摄像机欧拉角偏航角(角度)
    float cameraYaw;

    // 摄像机方向
    glm::vec3 cameraFront;
    // 摄像机上向量
    glm::vec3 cameraUp;
    // 摄像机右向量
    glm::vec3 cameraRight;

    // 摄像机FOV
    float cameraFOV;
    // 摄像机FOV最大值
    float cameraMaxFOV;

    // 摄像机移动速度
    float cameraSpeed;
    // 鼠标敏感度
    float mouseSensitivity;

    // 鼠标辅助变量,上一帧坐标X
    float lastX;
    // 鼠标辅助变量,上一帧坐标X
    float lastY;
    // 鼠标辅助变量,是否鼠标第一次进入窗口
    bool bIsMouseFirstIn;

public:
    // 构造函数
    Camera();

    // 获取视图矩阵
    glm::mat4 getViewMatrix();

    // 处理键盘输入
    void processKeyInput(CameraMovement movement, float delta);

    // 处理鼠标输入
    void processMouseInput(float x, float y);

    // 处理滚轮输入
    void processScrollInput(float yoffset);

private:
    // 处理摄像机方向、右向量、上向量
    void calcCameraVector();

public:
    // 设置摄像机的移动速度
    void setCameraSpeed(float cameraSpeed)
    {
        this->cameraSpeed = cameraSpeed;
    }
    // 设置摄像机的敏感度
    void setMouseSensitivity(float mouseSensitivity)
    {
        this->mouseSensitivity = mouseSensitivity;
    }
    // 设置摄像机最大FOV
    void setCameraMaxFOV(float cameraMaxFOV)
    {
        this->cameraMaxFOV = cameraMaxFOV;
    }
    // 获取摄像机FOV
    float getCameraFOV() const
    {
        return this->cameraFOV;
    }
};

#endif //OPENGLTUTORIAL_CAMERA_H
