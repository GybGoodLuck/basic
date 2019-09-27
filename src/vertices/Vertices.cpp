#include "Vertices.h"

VerticesBuffer Vertices::getVerticesBuffer(ObjectType type) {

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
            case SPHERE:
                buffer = bindSphere();
                std::cout << "bindSphere vao : " << buffer.vao << " vbo : " << buffer.vbo << std::endl;
                m_vaoMap.insert({SPHERE, buffer});
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

    buffer.indexCount = 6;

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

VerticesBuffer Vertices::bindFont() {
    VerticesBuffer buffer;

    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    buffer.indexCount = 6;
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

    buffer.indexCount = 36;

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

VerticesBuffer Vertices::bindSphere() {
    VerticesBuffer buffer;

    glGenVertexArrays(1, &buffer.vao);

    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    const int X_SEGMENTS = 64;
    const int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;

    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y       * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y       * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    buffer.indexCount = indices.size();

    std::vector<float> data;
    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }

    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    float stride = (3 + 2 + 3) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

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