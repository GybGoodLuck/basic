#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "../object/Font.h"

class Window {

public:
    Window(int width, int height, const Camera::Ptr& camera);

    void renderLoop();

    void addObject(const Object::Ptr& object);
    void removeObject(const Object::Ptr& object);

    int getFPS() { 
        return m_fps;
    }

private:
    int m_width;
    int m_height;

    bool isLeftDown = false;
    bool isRightDown = false;

    Camera::Ptr m_camera;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    int m_fps = 0;

    std::vector<Object::Ptr> m_renderList;

    GLFWwindow* m_GLWindow;

    void update(const Object::Ptr& object);

    void processInput();

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};