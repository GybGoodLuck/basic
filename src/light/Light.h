//
// Created by yuanbin on 19-8-26.
//

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {

public:
    using Ptr = std::shared_ptr<Light>;

    Light(glm::vec3 pos, glm::vec3 color);

    void setPos(glm::vec3 pos) {
        m_pos = pos;
    }

    glm::vec3 getPos() {
        return m_pos;
    }

    void setColor(glm::vec3 color) {
        m_color = color;
    }

    glm::vec3 getColor() {
        return m_color;
    }

private:
    glm::vec3 m_pos;
    glm::vec3 m_color;

};
