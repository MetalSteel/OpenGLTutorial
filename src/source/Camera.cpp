#include "Camera.h"

// 构造函数
Camera::Camera()
{
    // 摄像机位置
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    // 世界坐标上向量
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // 鼠标敏感度
    mouseSensitivity = 0.03f;
    // 摄像机FOV
    cameraFOV = 45.0f;
    // 摄像机FOV最大值
    cameraMaxFOV = 90.0f;
    // 摄像机移动速度
    cameraSpeed = 2.5f;
    // 摄像机欧拉角俯仰角
    cameraPitch = 0.0f;
    // 摄像机欧拉角偏航角
    cameraYaw = -90.0f;

    // 鼠标辅助变量,上一帧坐标X
    lastX = 0.0f;
    // 鼠标辅助变量,上一帧坐标X
    lastY = 0.0f;
    // 鼠标辅助变量,是否鼠标第一次进入窗口
    bIsMouseFirstIn = true;

    // 计算摄像机的向量属性
    calcCameraVector();
}

// 获取视图矩阵
glm::mat4 Camera::getViewMatrix()
{
    // 注意这里第二个参数是点,表示摄像机的观察点,而不是向量的意思,注意与CameraFront这个向量做区分
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

// 处理键盘输入
void Camera::processKeyInput(CameraMovement movement, float delta)
{
    // 这里要让速度与帧与帧之间的差值相乘,如果渲染一帧的时间比较短,则移动位置就比较小
    // 如果渲染一帧时间比较长则移动位置就比较大,这样才能保证在不同的机器上,位移的距离
    // 是一致的
    float velocity = cameraSpeed * delta;

    // 判断方向并在该方向上位移摄像机位置
    switch(movement)
    {
        case CameraMovement ::Forward:
            cameraPosition += cameraFront*velocity;
            break;
        case CameraMovement ::Backward:
            cameraPosition -= cameraFront*velocity;
            break;
        case CameraMovement ::Right:
            cameraPosition += cameraRight*velocity;
            break;
        case CameraMovement ::Left:
            cameraPosition -= cameraRight*velocity;
            break;
        default:
            break;
    }
}

// 处理鼠标输入
void Camera::processMouseInput(float x, float y)
{
    // 判断鼠标是否第一次进入窗口
    if(bIsMouseFirstIn)
    {
        lastX = x;
        lastY = y;
        bIsMouseFirstIn = false;
    }
    // 获取上一次鼠标的位置与当前鼠标位置的差值
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    // 保存此次鼠标位置作为最后一次位置
    lastX = x;
    lastY = y;
    // 降低鼠标的敏感度,防止鼠标位移太大
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    // 把鼠标的X方向位移差值作为Yaw角位移
    cameraYaw += xoffset;
    // 把鼠标的Y方向位移差值作为Pitch角位移
    cameraPitch += yoffset;
    // 重新计算摄像机的向量
    calcCameraVector();
}

// 处理鼠标滚动输入
void Camera::processScrollInput(float yoffset)
{
    // 判断当前FOV是否在合理位置,如果在合理位置再去更新FOV
    // 注意:鼠标的滚动轮的数值与要缩放的视角恰恰相反,因此要减去差值
    if(cameraFOV >= 1.0f && cameraFOV <= cameraMaxFOV)
        cameraFOV -= yoffset;
    // 判断是否FOV小于最小值或大于最大值
    if(cameraFOV <= 1.0f)
        cameraFOV = 1.0f;
    if(cameraFOV >= cameraMaxFOV)
        cameraFOV = cameraMaxFOV;
}

// 处理摄像机方向、右向量、上向量
void Camera::calcCameraVector()
{
    // 摄像机方向向量(欧拉角这里是角度,实际计算的cos/sin方法是弧度)
    glm::vec3 cameraFrontTmp;
    cameraFrontTmp.x = std::cos(glm::radians(cameraPitch)) * std::cos(glm::radians(cameraYaw));
    cameraFrontTmp.y = std::sin(glm::radians(cameraPitch));
    cameraFrontTmp.z = std::cos(glm::radians(cameraPitch)) * std::sin(glm::radians(cameraYaw));
    // 标准化摄像机方向向量
    cameraFront = glm::normalize(cameraFrontTmp);

    // 摄像机右向量
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));

    // 摄像机上向量
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}


