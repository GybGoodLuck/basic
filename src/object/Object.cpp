#include "Object.h"
#include "../Common.h"

Object::Object(const std::string& name, const Camera::Ptr& camera, 
        const ObjectAttribute& attribute, bool useLight, bool useReflect) 
        : m_name(name), m_camera(camera), m_attribute(attribute), 
        m_useLight(useLight), m_useReflect(useReflect) {    
    m_program = createProgram(verticesSource, fragmentSource);
    getUniformLocation();
}

void Object::getUniformLocation() {
    model = glGetUniformLocation(m_program, "model");
    view = glGetUniformLocation(m_program, "view");
    projection = glGetUniformLocation(m_program, "projection");
    color = glGetUniformLocation(m_program, "color");
    use_light = glGetUniformLocation(m_program, "useLight");
    use_reflect = glGetUniformLocation(m_program, "useReflect");
    camera_pos = glGetUniformLocation(m_program, "cameraPos");

    if (m_useLight) {
        light_size = glGetUniformLocation(m_program, "lightSize");
        light_pos = glGetUniformLocation(m_program, "lightPos");
        light_color = glGetUniformLocation(m_program, "lightColor");
        blinn = glGetUniformLocation(m_program, "blinn");
        gamma = glGetUniformLocation(m_program, "gamma");
    }
}

void Object::update() {
    glUseProgram(m_program);
    updateLocation();
    updateCamera();
    glUniform4f(color, m_attribute.color.x, m_attribute.color.y, m_attribute.color.z, m_attribute.alpha);

    if (m_useLight) {
        glUniform1i(use_light, true);
        glUniform1i(gamma, GammaCorrection);
        updateLight();
    } else {
        glUniform1i(use_light, false);
    }

    if (m_useReflect) {
        glUniform1i(use_reflect, true);
    } else {
        glUniform1i(use_reflect, false);
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

    glUniform3f(camera_pos, info.pos.x, info.pos.y, info.pos.z);
}

void Object::updateLight() {
    glUniform1i(light_size, LightManager::getInstance()->getSize());
    glUniform3fv(light_pos, LightManager::getInstance()->getSize(), (GLfloat*) LightManager::getInstance()->getPositions());
    glUniform3fv(light_color, LightManager::getInstance()->getSize(), (GLfloat*) LightManager::getInstance()->getColors());
    glUniform1i(blinn, m_blinn);
}
