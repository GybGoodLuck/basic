#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <map>

#include "VerticesInfo.h"

struct VerticesBuffer {
    VAO vao = 0;
    VBO vbo = 0;

    bool isVaild() {
        return (vao != 0) && (vbo != 0);
    }
};

class Vertices {

public:
    static std::shared_ptr<Vertices> getInstance();
    VerticesBuffer getVAO(VerticesType type);


private:
    std::map<VerticesType, VerticesBuffer> m_vaoMap;

    VerticesBuffer bindPlane();
    VerticesBuffer bindFont();
    VerticesBuffer findVAO(VerticesType type);
};