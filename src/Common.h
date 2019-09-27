#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const unsigned int SCR_WIDTH = 1600;
static const unsigned int SCR_HEIGHT = 1200;

static const bool GammaCorrection = true;

enum ObjectType {
    TRIANGLE = 0,
    FONT,
    PLANE,
    CUBE,
    SPHERE,
    MODEL,
    MESH
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;
};

struct ObjectAttribute {
    glm::vec3 pos = {0.0f, 0.0f, 1.0f};
    glm::quat quat = {0.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    float alpha = 1.0f;

    int textureID;
};

struct MeshData {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
};