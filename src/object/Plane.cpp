#include "Plane.h"

void Plane::init() {
    m_vao = Vertices::getInstance()->getVerticesBuffer(type()).vao;
    m_vbo = Vertices::getInstance()->getVerticesBuffer(type()).vbo;
    m_indexCount = Vertices::getInstance()->getVerticesBuffer(type()).indexCount;
}

void Plane::render() {
    glBindTexture(GL_TEXTURE_2D, m_attribute.textureID);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_indexCount);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}