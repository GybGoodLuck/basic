#pragma once

#include <vector>
#include <iostream>

#include "../object/Plane.h"

class Window {

public:
    Window(int width, int height);

    void renderLoop();

    void addObject(const std::shared_ptr<Object>& object);
    void removeObject(const std::shared_ptr<Object>& object);

private:
    int m_width;
    int m_height;

    std::vector<std::shared_ptr<Object>> m_renderList;

    GLFWwindow* m_GLWindow;
};