#pragma once

#include <map>
#include <mutex>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../GLUtils.h"

struct Character {
    GLuint     TextureID;  // 字形纹理的ID
    glm::ivec2 Size;       // 字形大小
    glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
    GLuint     Advance;    // 原点距下一个字形原点的距离
};

class Characters {

public:
    Characters();
    static std::shared_ptr<Characters> getInstance();
    Character getCharacter(GLchar c);

private:
    std::map<GLchar, Character> m_characters;
    FT_Face m_face;
    
    void loadCharacters();

};