#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ObjectType {
    TRIANGLE = 0,
    FONT,
    PLANE,
    CUBE,
};

struct ObjectAttribute {
    glm::vec3 pos = {0.0f, 0.0f, 1.0f};
    glm::quat quat = {0.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    float alpha = 1.0f;

    GLuint textureID;
};