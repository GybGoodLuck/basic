#pragma once

#include "../vertices/Vertices.h"
#include "../shader/Shader.h"
#include "../GLUtils.h"

class Object {

public:
    Object(const std::string& name);
    virtual ~Object() {};

    virtual ObjectType type() = 0;
    virtual void render() = 0;

protected:
    std::string m_name;

    VAO m_vao;
    VBO m_vbo;

    GLuint m_program;
    GLuint m_texture;

    glm::mat4 m_projection;
};