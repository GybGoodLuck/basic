#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <map>

#include "VerticesInfo.h"

class Vertices {

public:
    static std::shared_ptr<Vertices> getInstance();
    VAO getVAO(VerticesType type);


private:
    std::map<VerticesType, VAO> m_vaoMap;

    VAO bindPlane();
    VAO findVAO(VerticesType type);
};