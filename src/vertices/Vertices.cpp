#include "Vertices.h"

VerticesBuffer Vertices::getVAO(ObjectType type) {
    auto buffer = findVAO(type);

    if (!buffer.isVaild()) {
        switch (type)
        {
            case FONT:
                buffer = bindFont();
                std::cout << "bindFont vao : " << buffer.vao << " vbo : " << buffer.vbo << std::endl;
                m_vaoMap.insert({FONT, buffer});
                break;
            case PLANE:
                buffer = bindPlane();
                std::cout << "bindPlane vao : " << buffer.vao << " vbo : " << buffer.vbo << std::endl;
                m_vaoMap.insert({PLANE, buffer});
                break;
            case CUBE:
                buffer = bindCube();
                std::cout << "bindCube vao : " << buffer.vao << " vbo : " << buffer.vbo << std::endl;
                m_vaoMap.insert({CUBE, buffer});
                break;
            default:
                break;
        }
    }

    return buffer;
}

VerticesBuffer Vertices::findVAO(ObjectType type) {
    VerticesBuffer buffer;

    auto it = m_vaoMap.find(type);

    if (it != m_vaoMap.end()) {
        return it->second;
    } 

    return buffer;
}

VerticesBuffer Vertices::bindPlane() {
    VerticesBuffer buffer;

    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);

    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return buffer;
}

VerticesBuffer Vertices::bindFont() {
    VerticesBuffer buffer;

    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return buffer;
}

VerticesBuffer Vertices::bindCube() {
    VerticesBuffer buffer;

    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);

    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return buffer;
}

std::shared_ptr<Vertices> Vertices::getInstance() {
    static std::shared_ptr<Vertices> s_vertices;
    static std::mutex s_mutex;
    if (s_vertices) return s_vertices; 
    std::lock_guard<std::mutex> locker(s_mutex);
    if (!s_vertices) {
        s_vertices = std::make_shared<Vertices>();
    }
    return s_vertices;
}