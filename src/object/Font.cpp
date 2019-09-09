#include "Font.h"

Font::Font(const std::string& name, const Camera::Ptr& camera, const ObjectAttribute& attribute)
        : Object(name, camera, attribute) {
    m_program = createProgram(fontVerticesSource, fontFragmentSource);
    m_vao = Vertices::getInstance()->getVAO(type()).vao;
    m_vbo = Vertices::getInstance()->getVAO(type()).vbo;
    getUniformLocation();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
}

void Font::update() {
    glUseProgram(m_program);
    m_projection = glm::ortho(0.0f, static_cast<GLfloat>(800.f), 0.0f, static_cast<GLfloat>(600.f));
    glUniform4f(color, m_attribute.color.x, m_attribute.color.y, m_attribute.color.z, m_attribute.alpha);
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_projection));
}

void Font::render() {
    renderText(m_text, m_attribute.pos.x, m_attribute.pos.y, m_attribute.scale.x);
}

void Font::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
    // Activate corresponding render state
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