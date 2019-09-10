#include <GLFW/glfw3.h>

#include "Controller.h"

void Controller::setMousePosition(MousePosition position) {
    m_mousePosition = position;

    auto it = m_mouseStates.find(GLFW_MOUSE_BUTTON_RIGHT);

    if (it != m_mouseStates.end()) {
        auto lastState = it->second;
        switch (lastState.action)
        {
            case GLFW_PRESS:
                {
                    float xoffset = m_mousePosition.x - lastState.position.x;
                    float yoffset = lastState.position.y - m_mousePosition.y;
                    m_camera->processMouseMovement(xoffset, yoffset);
                }
                break;
            case GLFW_RELEASE:
                break;
            default:
                break;
        }

        lastState.position = m_mousePosition;
        m_mouseStates[GLFW_MOUSE_BUTTON_RIGHT] = lastState;
    }
};

void Controller::processMouse(int button, int action, int mods) {
    MouseState state;
    state.action = action;
    state.button = button;
    state.position = m_mousePosition;
    m_mouseStates[button] = state;
}

std::shared_ptr<Controller> Controller::getInstance() {
    static std::shared_ptr<Controller> s_controller;
    static std::mutex s_mutex;
    if (s_controller) return s_controller;
    std::lock_guard<std::mutex> locker(s_mutex);
    if (!s_controller) {
        s_controller = std::make_shared<Controller>();
    }
    return s_controller;
}