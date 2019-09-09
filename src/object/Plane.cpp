#include "Plane.h"
#include "../characters/Characters.h"

Plane::Plane(const std::string& name, const Camera::Ptr& camera, const ObjectAttribute& attribute)
         : Object(name, camera, attribute) {
    m_program = createProgram(verticesSource, fragmentSource);
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
    getUniformLocation();
}

void Plane::update() {
    glUseProgram(m_program);
    updateLocation();
    updateCamera();
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniform4f(color, m_attribute.color.x, m_attribute.color.y, m_attribute.color.z, m_attribute.alpha);
}

void Plane::render() {
    glBindTexture(GL_TEXTURE_2D, m_attribute.textureID);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}