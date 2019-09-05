#include "Vertices.h"

VAO Vertices::getVAO(VerticesType type) {
    auto vao = findVAO(type);

    if (!vao) {
        switch (type)
        {
            case PLANE:
                vao = bindPlane();
                std::cout << "bindPlane vao : " << vao << std::endl;
                m_vaoMap.insert({PLANE, vao});
                break;
            default:
                break;
        }
    }

    return vao;
}

VAO Vertices::findVAO(VerticesType type) {
    VAO vao = 0;

    auto it = m_vaoMap.find(type);

    if (it != m_vaoMap.end()) {
        return it->second;
    } 

    return vao;
}

VAO Vertices::bindPlane() {
    VAO vao;
    VBO vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    return vao;
}

static std::shared_ptr<Vertices> s_vertices;
static std::mutex s_mutex;

std::shared_ptr<Vertices> Vertices::getInstance() {
    if (s_vertices) return s_vertices; 
    std::lock_guard<std::mutex> locker(s_mutex);
    if (!s_vertices) {
        s_vertices = std::make_shared<Vertices>();
    }
    return s_vertices;
}