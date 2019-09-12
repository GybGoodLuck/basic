//
// Created by yuanbin on 19-8-26.
//

#include "LightManager.h"

void LightManager::addLight(int id, const Light::Ptr light) {

    if (m_lights.size() == 10) {
        std::cout << "can not add light any more!" << std::endl;
    } else {
        m_lights.insert({id, light});
        updateData();
    }
}

void LightManager::removeLight(int id) {

    auto it = m_lights.find(id);

    if (it != m_lights.end())
    {
        m_lights.erase(it);
        updateData();
    }
}

void LightManager::updateData() {
    int i = 0;

    for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
        m_poses[i] = it->second->getPos();
        m_colors[i] = it->second->getColor();
        i += 1;
    }
}

std::shared_ptr<LightManager> LightManager::getInstance() {
    static std::shared_ptr<LightManager> s_lightManager;
    static std::mutex s_mutex;
    if (s_lightManager) return s_lightManager;
    std::lock_guard<std::mutex> locker(s_mutex);
    if (!s_lightManager) {
        s_lightManager = std::make_shared<LightManager>();
    }
    return s_lightManager;
}