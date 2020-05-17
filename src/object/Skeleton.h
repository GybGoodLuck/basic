#pragma once

#include "Node.h"

class Skeleton {

public:
    typedef std::shared_ptr<Skeleton> Ptr;
    Skeleton() { maxBoneId = 0;}

    void addBone(const Node::Ptr& bone) {

        auto it = m_bones.begin();
        for (; it != m_bones.end(); it++) {

            if (bone->getName() == (*it)->getName()) {
                return;
            }
        }

        bone->setIsBone(true);
        bone->setBoneId(maxBoneId++);
        m_bones.push_back(bone);
    }

    Node::Ptr getBoneByName(const std::string& name) {
        
        auto it = m_bones.begin();
        for (; it != m_bones.end(); it++) {
            auto bone = *it;

            if (name == bone->getName()) {
                return bone;
            }
        }

        return nullptr;
    }

    int getMaxBones() {
        return m_bones.size();
    }

    std::vector<glm::mat4> getBoneTransforms() {

        std::vector<glm::mat4> boneTransforms;

        for (unsigned int i = 0; i < m_bones.size(); i++) {
            boneTransforms.push_back(m_bones[i]->getBoneTransformation());
        }

        return boneTransforms;
    }

private:
    uint maxBoneId;
    std::vector<Node::Ptr> m_bones;
};