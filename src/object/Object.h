#pragma once

#include "../vertices/Vertices.h"
#include "../shader/Shader.h"
#include "../GLUtils.h"

class Object {

public:
    Object();
    virtual ~Object() {};

    virtual void render() = 0;

protected:
    GLuint m_program;
    VAO m_vao;
    GLuint m_texture;

};