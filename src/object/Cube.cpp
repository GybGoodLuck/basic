#include "Cube.h"

void Cube::init() {
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
}

void Cube::render() {
    glBindTexture(GL_TEXTURE_2D, m_attribute.textureID);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}