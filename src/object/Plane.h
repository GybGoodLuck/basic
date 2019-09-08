#pragma once

#include "Object.h"

class Plane : public Object {

public:
    Plane(const std::string& name, const Camera::Ptr& camera);

    ObjectType type() override {
        return PLANE;
    }

    void render() override;
    void update() override;
};