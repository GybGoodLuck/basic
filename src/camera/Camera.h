#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct Projection {
    float near = 0.1f;
    float far = 200.f;
    float fov = glm::radians(45.f);
    float aspect = 1.0f;
};

struct CameraInfo
{
    glm::vec3 pos = {0.0f, 0.2f, 0.0f};
    glm::quat quat = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec3 dir = {0.0f, 0.0f, 1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
};

class Camera
{
public:
    using Ptr = std::shared_ptr<Camera>;

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

    void setCameraInfo(const CameraInfo& cameraInfo) {
        m_cameraInfo = cameraInfo;
    }

    CameraInfo getCameraInfo() {
        return m_cameraInfo;
    }
    
    void setProjection(const Projection& projection) {
        m_projection = projection;
    }

    Projection getProjection() const {
        return m_projection;
    }
    
private:
    CameraInfo m_cameraInfo;
    Projection m_projection;

    float m_speed = 0.5f;
    float m_rotateSpeed = 0.225f;
};
