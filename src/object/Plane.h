#pragma once

#include "Object.h"

class Plane : public Object {

public:
    Plane(const std::string& name);

    ObjectType type() override {
        return PLANE;
    }

    void render() override;

};