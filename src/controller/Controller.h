#pragma once

#include <mutex>
#include <memory>

#include <map>

#include "../camera/Camera.h"

struct MousePosition {
    float x;
    float y;
};

struct MouseState {
    int button;
    int action;
    MousePosition position;
};

class Controller {

public:
    using Ptr = std::shared_ptr<Controller>;
    static std::shared_ptr<Controller> getInstance();

    void setCamera(const Camera::Ptr& camera) {
        m_camera = camera;
    };

    void setMousePosition(MousePosition position);

    void processMouse(int button, int action, int mods);

private:
    Camera::Ptr m_camera;
    MousePosition m_mousePosition;

    std::map<int, MouseState> m_mouseStates;
};