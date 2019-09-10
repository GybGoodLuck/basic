#include "Camera.h"

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    glm::vec3 Front = m_cameraInfo.dir;
    glm::vec3 Right = glm::normalize(glm::cross(Front, m_cameraInfo.up));

    float velocity = 0.2 * deltaTime;
    if (direction == FORWARD)
        m_cameraInfo.pos += Front * velocity;
    if (direction == BACKWARD)
        m_cameraInfo.pos -= Front * velocity;
    if (direction == LEFT)
        m_cameraInfo.pos -= Right * velocity;
    if (direction == RIGHT)
        m_cameraInfo.pos += Right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    float angleX = 0.1 * xoffset;
    auto rotateX = glm::angleAxis(glm::radians(angleX), axisY);
    
    m_cameraInfo.dir = m_cameraInfo.dir * rotateX;
}