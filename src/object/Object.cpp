#include "Object.h"

Object::Object(const std::string& name, const Camera::Ptr& camera) 
        : m_name(name), m_camera(camera) {}

void Object::getUniformLocation() {
    model = glGetUniformLocation(m_program, "model");
    view = glGetUniformLocation(m_program, "view");
    projection = glGetUniformLocation(m_program, "projection");
}

void Object::updateLocation() {
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_pos);
    m_model = m_model * glm::mat4_cast(m_quat);
}

void Object::updateCamera() {
    auto pro = m_camera->getProjection();
    m_projection = glm::perspective(pro.fov, pro.aspect,
        pro.near, pro.far);

    auto info = m_camera->getCameraInfo();
    m_view = glm::lookAt(info.pos, info.pos + info.dir, info.up);
}