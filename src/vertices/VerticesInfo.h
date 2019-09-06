#pragma once

typedef unsigned int VAO;
typedef unsigned int VBO;
typedef unsigned int EBO;

enum VerticesType {
    TRIANGLE = 0,
    FONT,
    PLANE,
    Cube,
};

static const float planeVertices[] = {           
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f,

     0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f
};