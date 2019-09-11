#pragma once

#include "Object.h"

class Cube : public Object {

public:
    Cube(const std::string& name, const Camera::Ptr& camera, const ObjectAttribute& attribute);

    ObjectType type() override {
        return CUBE;
    }

    void render() override;
    void update() override;
};