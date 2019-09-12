//
// Created by yuanbin on 19-8-26.
//

#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Light {

public:
    using Ptr = std::shared_ptr<Light>;

    Light(glm::vec3 pos, glm::vec3 color) : m_pos(pos), m_color(color) {}

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

class LightManager {

public:
    static std::shared_ptr<LightManager> getInstance();

    void addLight(int id, const Light::Ptr light);
    void removeLight(int id);

    void updateData();

    glm::vec3* getPositions() {
        return m_poses;
    }

    glm::vec3* getColors() {
        return m_colors;
    }

    int getSize() {
        return m_lights.size();
    }

private:
    std::map<int, Light::Ptr> m_lights;

    glm::vec3 m_poses[10];
    glm::vec3 m_colors[10];

};