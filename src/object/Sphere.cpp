#include "Sphere.h"

void Sphere::init() {
    m_vao = Vertices::getInstance()->getVerticesBuffer(type()).vao;
    m_vbo = Vertices::getInstance()->getVerticesBuffer(type()).vbo;
    m_indexCount = Vertices::getInstance()->getVerticesBuffer(type()).indexCount;
}

void Sphere::render() {
    glBindTexture(GL_TEXTURE_2D, m_attribute.textureID);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}