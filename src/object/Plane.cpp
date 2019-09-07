#include "Plane.h"
#include "../characters/Characters.h"
#include <Config.h>

Plane::Plane(const std::string& name) : Object(name) {
    m_program = createProgram(verticesSource, fragmentSource);
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
    std::string path = "image/yzq.jpg";
    path = RES_PATH + path;
    m_texture = loadTexture(path.c_str());
}

void Plane::render() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}