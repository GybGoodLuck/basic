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
    void updateBones() override;

    MeshData getData() const {
        return m_data;
    }

    void setData(const MeshData& data) {
        m_data = data;
    }

    void setTransformation(const glm::mat4& trans) {
        m_data.transformation = trans;
    }

    void setBoneTransformation(int boneID, const glm::mat4& boneTransformation) {
        if (m_boneTransformations.size() == 0) m_boneTransformations.resize(10);
        m_boneTransformations[boneID] = boneTransformation;
    }

    void setGlobals(int boneID, const glm::mat4& global) {
        if (m_globals.size() == 0) m_globals.resize(10);
        m_globals[boneID] = global;
    }
    
private:
    MeshData m_data;
    std::vector<glm::mat4> m_boneTransformations;
    std::vector<glm::mat4> m_globals;
};