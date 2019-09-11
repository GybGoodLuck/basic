#pragma once

#include "Object.h"

class Plane : public Object {

public:
    using Object::Object;
    using Object::update;

    ObjectType type() override {
        return PLANE;
    }

    void init() override;
    void render() override;
};