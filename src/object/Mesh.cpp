#include "Mesh.h"

void Mesh::init() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenBuffers(1, &m_bbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_data.vertices.size() * sizeof(Vertex), &m_data.vertices[0], GL_STATIC_DRAW);  

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.indices.size() * sizeof(unsigned int), &m_data.indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_bbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoneData) * m_data.boneDatas.size(), &m_data.boneDatas[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(BoneData), (const GLvoid*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(BoneData), (const GLvoid*)16);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_indexCount = m_data.indices.size();
    m_attribute.color = m_data.color;
}

void Mesh::render() {

    for (auto texture : m_data.textures) {
        if (texture.type == "texture_diffuse")
            glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::updateBones() {
    glUniformMatrix4fv(global, 1, GL_TRUE, glm::value_ptr(m_data.transformation));

    if (m_boneTransformations.size() == 0) return;
    glUniformMatrix4fv(bones, m_boneTransformations.size(), GL_TRUE, (const GLfloat*)(&m_boneTransformations[0]));
    glUniformMatrix4fv(globals, m_globals.size(), GL_TRUE, (const GLfloat*)(&m_globals[0]));
}