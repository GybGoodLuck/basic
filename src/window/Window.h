#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "../object/Font.h"

class Window {

public:
    Window(int width, int height);

    void renderLoop();

    void addObject(const std::shared_ptr<Object>& object);
    void removeObject(const std::shared_ptr<Object>& object);

    int getFPS() { 
        return m_fps;
    }

private:
    int m_width;
    int m_height;

    int m_fps;

    std::vector<std::shared_ptr<Object>> m_renderList;

    GLFWwindow* m_GLWindow;

    void update(const std::shared_ptr<Object>& object);
};