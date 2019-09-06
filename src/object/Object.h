#pragma once

#include "../vertices/Vertices.h"
#include "../shader/Shader.h"
#include "../GLUtils.h"

class Object {

public:
    Object();
    virtual ~Object() {};

    virtual void render() = 0;

    virtual void setText(const std::string& text) {
        m_text = text;
    }

protected:
    GLuint m_program;
    VAO m_vao;
    VBO m_vbo;
    GLuint m_texture;

    glm::mat4 m_projection;

    std::string m_text = "FPS ";

};