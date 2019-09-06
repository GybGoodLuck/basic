#pragma once

#include "Object.h"
#include "../characters/Characters.h"

class Font : public Object {

public:
    Font();

    void render() override;

private:
    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};