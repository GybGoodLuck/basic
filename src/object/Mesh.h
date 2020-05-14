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
        if (m_boneTransformations.size() == 0) {
            m_boneTransformations.resize(100);
            for (int i = 0; i < 100; i++) {
                m_boneTransformations[i] = glm::mat4(1.0f);
            }
        }
        m_boneTransformations[boneID] = boneTransformation;
    }

    void addBone(const std::string& name) {

        auto it = m_bones.find(name);

        if (it == m_bones.end()) {
            m_bones[name] = boneId++;
        }
    }

    int findBone(const std::string& name) {
        auto it = m_bones.find(name);
        if (it != m_bones.end()) return it->second;
        return -1;
    }

    void addBoneOffsets(uint boneId, const glm::mat4& offset) {
        m_boneOffsets[boneId] = offset;
    }

    glm::mat4 getBoneOffset(uint boneId) {
        auto it = m_boneOffsets.find(boneId);
        if (it != m_boneOffsets.end()) return it->second;
        return glm::mat4(1.0f);
    }
    
private:
    uint boneId = 0;
    MeshData m_data;
    std::map<uint, glm::mat4> m_boneOffsets;
    std::vector<glm::mat4> m_boneTransformations;
    std::map<std::string, uint> m_bones;
};