#include "Object.h"

Object::Object(const std::string& name, const Camera::Ptr& camera, const Light::Ptr& light, const ObjectAttribute& attribute) 
        : m_name(name), m_camera(camera), m_attribute(attribute), m_light(light), m_useLight(true) {    
    m_program = createProgram(verticesSource, fragmentSource);
    getUniformLocation();
}

Object::Object(const std::string& name, const Camera::Ptr& camera, const ObjectAttribute& attribute) 
        : m_name(name), m_camera(camera), m_attribute(attribute), m_useLight(false) {
    m_program = createProgram(verticesSource, fragmentSource);
    getUniformLocation();
}

void Object::getUniformLocation() {
    model = glGetUniformLocation(m_program, "model");
    view = glGetUniformLocation(m_program, "view");
    projection = glGetUniformLocation(m_program, "projection");
    color = glGetUniformLocation(m_program, "color");
    use_light = glGetUniformLocation(m_program, "useLight");

    if (m_useLight) {
        light_pos = glGetUniformLocation(m_program, "lightPos");
        light_color = glGetUniformLocation(m_program, "lightColor");
        blinn = glGetUniformLocation(m_program, "blinn");
        camera_pos = glGetUniformLocation(m_program, "cameraPos");
    }
}

void Object::update() {
    glUseProgram(m_program);
    updateLocation();
    updateCamera();
    glUniform4f(color, m_attribute.color.x, m_attribute.color.y, m_attribute.color.z, m_attribute.alpha);

    if (m_useLight) {
        glUniform1i(use_light, true);
        updateLight();
    } else {
        glUniform1i(use_light, false);
    }
}

void Object::updateLocation() {
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_attribute.pos);
    m_model = glm::scale(m_model, m_attribute.scale);
    m_model = m_model * glm::mat4_cast(m_attribute.quat);
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m_model));
}

void Object::updateCamera() {
    auto pro = m_camera->getProjection();
    m_projection = glm::perspective(pro.fov, pro.aspect,
        pro.near, pro.far);

    auto info = m_camera->getCameraInfo();
    m_view = glm::lookAt(info.pos, info.pos + info.dir, info.up);

    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m_view));
}

void Object::updateLight() {
    auto lightPos = m_light->getPos();
    auto lightColor = m_light->getColor();
    auto cameraPos = m_camera->getCameraInfo().pos;
    glUniform3f(light_pos, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(light_color, lightColor.x, lightColor.y, lightColor.z);
    glUniform1i(blinn, m_blinn);
    glUniform3f(camera_pos, cameraPos.x, cameraPos.y, cameraPos.z);
}