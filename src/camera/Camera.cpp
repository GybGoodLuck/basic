#include "Camera.h"

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    glm::vec3 Front = m_cameraInfo.dir;
    Front.y = 0.0f;
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

    float angleX = 0.2 * xoffset;
    float angleY = 0.2 * yoffset;

    glm::vec3 axisX = {1.0f, 0.0f, 0.0f};
    glm::vec3 axisY = {0.0f, 1.0f, 0.0f};

    auto rotate = glm::angleAxis(glm::radians(angleX), axisY);
    rotate *= glm::angleAxis(glm::radians(angleY), axisX);
    
    m_cameraInfo.dir = m_cameraInfo.dir * rotate;
}