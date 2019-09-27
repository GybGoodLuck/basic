#pragma once

#include "Object.h"

class Mesh : public Object {

public:
    using Object::Object;
    using Object::update;

    ObjectType type() override {
        return MESH;
    }

    void init() override;
    void render() override;

    void setData(const MeshData& data) {
        m_data = data;
    };

private:
    MeshData m_data;

};