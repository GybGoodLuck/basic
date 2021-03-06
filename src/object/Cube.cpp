#include "Cube.h"

void Cube::init() {
    m_vao = Vertices::getInstance()->getVerticesBuffer(type()).vao;
    m_vbo = Vertices::getInstance()->getVerticesBuffer(type()).vbo;
    m_indexCount = Vertices::getInstance()->getVerticesBuffer(type()).indexCount;
}

void Cube::render() {

    if (m_useReflect) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_attribute.envTextureID);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_attribute.textureID);
    }
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_indexCount);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}