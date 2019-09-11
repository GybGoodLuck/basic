#pragma once

#include "Object.h"
#include "../characters/Characters.h"

class Font : public Object {

public:
    Font(const std::string& name, const Camera::Ptr& camera, const ObjectAttribute& attribute);

    ObjectType type() override {
        return FONT;
    }

    void init() override;
    void render() override;
    void update() override;

    void setText(const std::string& text) {
        m_text = text;
    }

    std::string getText() const {
        return m_text;
    }

private:
    std::string m_text = "FPS ";

    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
};