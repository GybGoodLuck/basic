#include "Characters.h"

#include <Config.h>

Characters::Characters() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    std::string path = "fonts/NotoSansCJK-Black.ttc";
    path = RES_PATH + path;

    if (FT_New_Face(ft, path.c_str(), 0, &m_face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(m_face, 0, 48);
    loadCharacters();

    FT_Done_Face(m_face);
    FT_Done_FreeType(ft);
}

void Characters::loadCharacters() {

    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形 
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // 生成纹理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_face->glyph->bitmap.width,
            m_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_face->glyph->bitmap.buffer
        );
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture, 
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            static_cast<GLuint>(m_face->glyph->advance.x)
        };
        m_characters.insert(std::pair<GLchar, Character>(c, character));
    }
    std::cout << "load characters finish !" << std::endl;
}

Character Characters::getCharacter(GLchar c) {
    auto it = m_characters.find(c);

    if (it != m_characters.end()) {
        return it->second;
    }
    return Character();
}


std::shared_ptr<Characters> Characters::getInstance() {
    static std::shared_ptr<Characters> s_characters;
    static std::mutex s_mutex;
    if (s_characters) return s_characters;
    std::lock_guard<std::mutex> locker(s_mutex);
    if (!s_characters) {
        s_characters = std::make_shared<Characters>();
    }
    return s_characters;
}