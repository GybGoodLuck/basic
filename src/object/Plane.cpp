#include "Plane.h"
#include "../characters/Characters.h"
#include <Config.h>

Plane::Plane(const std::string& name, const Camera::Ptr& camera)
         : Object(name, camera) {
    m_program = createProgram(verticesSource, fragmentSource);
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
    getUniformLocation();
    std::string path = "image/yzq.jpg";
    path = RES_PATH + path;
    m_texture = loadTexture(path.c_str());
}

void Plane::update() {
    glUseProgram(m_program);
    updateLocation();
    updateCamera();
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m_model));
}

void Plane::render() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}