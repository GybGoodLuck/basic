#pragma once

#include "Object.h"

class Cube : public Object {

public:
    using Object::Object;
    using Object::update;

    ObjectType type() override {
        return CUBE;
    }

    void init() override;
    void render() override;
};