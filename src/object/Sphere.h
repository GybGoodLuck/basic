#pragma once

#include "Object.h"

class Sphere : public Object {

public:
    using Object::Object;
    using Object::update;

    ObjectType type() override {
        return SPHERE;
    }

    void init() override;
    void render() override;
};