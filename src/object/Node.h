#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Common.h"
#include "Mesh.h"

struct VectorKey {
    double time;
    glm::vec3 value;
};

struct QuatKey {
    double time;
    glm::quat value;
};

struct NodeAnimation {
    std::vector<VectorKey> positionKeys;
    std::vector<QuatKey> rotationKeys;
    std::vector<VectorKey> scaleKeys;
};

class Node {

public:
    typedef std::shared_ptr<Node> Ptr;  
    Node(const std::string& name, const glm::mat4& transformation = glm::mat4(1.0f)) 
            : m_name(name), m_transformation(transformation), hasAnimation(false), m_isBone(false) { }

    std::string getName() const {
        return m_name;
    }

    void setTransformation(const glm::mat4& transformation) {
        m_transformation = transformation;
    }

    glm::mat4 getTransformation() const { 
        return m_transformation;
    }

    void setWorldTransformation(const glm::mat4& transformation) {
        m_worldTransformation = transformation;

        for (auto mesh : m_meshes) {
            mesh->setGlobalTransform(m_worldTransformation);
        }
    }

    glm::mat4 getWorldTransformation() const { 
        return m_worldTransformation;
    }

    void setParent(const Node::Ptr parent) {
        m_parent = parent;
    }

    Node::Ptr getParent() const {
        return m_parent;
    }

    void setIsBone(bool isBone) {
        m_isBone = isBone;
    }

    bool isBone() {
        return m_isBone;
    }

    void addChild(const Node::Ptr& child) {
        m_children.push_back(child);
    }

    std::vector<Node::Ptr> getChildren() const {
        return m_children;
    }

    void setNodeAnimation(const NodeAnimation& animation) {
        hasAnimation = true;
        m_animation = animation;
    }

    bool hasNodeAnimation() {
        return hasAnimation;
    }

    glm::mat4 getTransByAnimTime(float animTime);

    void setBoneId(uint id) {
        m_boneId = id;
    }

    uint getBoneId() {
        return m_boneId;
    }

    glm::mat4 getBoneTransformation() const { 
        return m_worldTransformation;
    }

    Node::Ptr getChildrenByName(const std::string& name);

    void addMesh(const Mesh::Ptr& mesh) {
        m_meshes.push_back(mesh);
    }

private:
    std::string m_name;
    glm::mat4 m_transformation;
    glm::mat4 m_worldTransformation;
    NodeAnimation m_animation;
    bool hasAnimation;

    bool m_isBone;
    uint m_boneId;

    Node::Ptr m_parent;
    std::vector<Node::Ptr> m_children;
    std::vector<Mesh::Ptr> m_meshes;

    void calcInterpolatedPosition(aiVector3D& Out, float animationTime, const NodeAnimation& nodeAnimation);
    void calcInterpolatedRotation(aiQuaternion& Out, float animationTime, const NodeAnimation& nodeAnimation);
    void calcInterpolatedScaling(aiVector3D& Out, float animationTime, const NodeAnimation& nodeAnimation);
    uint findRotation(float animationTime, const NodeAnimation& nodeAnimation);
    uint findPosition(float animationTime, const NodeAnimation& nodeAnimation);
    uint findScaling(float animationTime, const NodeAnimation& nodeAnimation);
};