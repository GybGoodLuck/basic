#pragma once

#include "../camera/Camera.h"
#include "../vertices/Vertices.h"
#include "../shader/Shader.h"
#include "../GLUtils.h"

class Object {

public:
    Object(const std::string& name, const Camera::Ptr& camera);
    virtual ~Object() {};

    virtual ObjectType type() = 0;
    virtual void render() = 0;
    virtual void update() = 0;

    void setQua(const glm::quat& qua) {
        m_quat = qua;
    };

    glm::quat getQua() {
        return m_quat;
    };

protected:
    std::string m_name;

    glm::vec3 m_pos = {0.0f, 0.0f, 3.0f};
    glm::quat m_quat = {0.0f, 0.0f, 0.0f, 1.0f};

    VAO m_vao;
    VBO m_vbo;

    GLuint m_program;
    GLuint m_texture;

    Camera::Ptr m_camera;

    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_model;

    glm::vec3 m_color;

    GLint projection;
    GLint view;
    GLint model;

    void getUniformLocation();
    void updateLocation();
    void updateCamera();
};