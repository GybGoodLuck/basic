#pragma once

#include "Object.h"

class Mesh : public Object {

public:
    typedef std::shared_ptr<Mesh> Ptr;
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

    void setBoneTransformations(const std::vector<glm::mat4>& boneTransformations) {
        auto bs = boneTransformations;
        for (auto it = m_boneOffsets.begin(); it != m_boneOffsets.end(); it++) {
            bs[it->first] = it->second * bs[it->first];
        }
        m_boneTransformations = bs;
    }

    void setGlobalTransform(const glm::mat4& globalTransform) {
        m_globalTransform = globalTransform;
    }

    void setBoneOffset(int boneId, const glm::mat4& boneOffset) {
        m_boneOffsets[boneId] = boneOffset;
    }
    
private:
    MeshData m_data;
    glm::mat4 m_globalTransform;
    std::map<int, glm::mat4> m_boneOffsets;
    std::vector<glm::mat4> m_boneTransformations;
};