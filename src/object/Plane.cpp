#include "Plane.h"
#include <stdio.h>

Plane::Plane() {
    m_program = createProgram(verticesSource, fragmentSource);
    m_vao = Vertices::getInstance()->getVAO(PLANE);
    m_texture = loadTexture("/home/yuanbin/opengl/workspace/basic/resource/image/yzq.jpg");
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Plane::render() {
    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}