#include "Font.h"

Font::Font(const std::string& name) : Object(name) {
    m_program = createProgram(fontVerticesSource, fontFragmentSource);
    m_projection = glm::mat4(1.0f);
    m_projection = glm::scale(m_projection, {0.01f, 0.01f, 0.01f});
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
}

void Font::render() {
    renderText(m_text, -95.0f, 85.0f, 0.15f, glm::vec3(0.2, 0.2f, 0.8f));
}

void Font::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    glUseProgram(m_program);
    auto textColor = glGetUniformLocation(m_program, "textColor");
    glUniform3f(textColor, color.x, color.y, color.z);
    auto projection = glGetUniformLocation(m_program, "projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters::getInstance()->getCharacter(*c);

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}