#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Common.h"
#include "../GLUtils.h"

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

struct NodeData {
    std::string name;
    glm::mat4 transformation;
    bool hasNodeAnimation;
    NodeAnimation nodeAnimation;
    std::vector<std::shared_ptr<NodeData>> children;
};

class Model : public Object {

public:
    using Object::update;

    Model(const std::string& name, const Camera::Ptr& camera,
        const ObjectAttribute& attribute, const std::string& path, bool useLight = true);

    ObjectType type() override {
        return MODEL;
    }

    void init() override;
    void render() override;

    std::vector<Texture> getTextures() {
        return m_textures;
    }

    std::string getDirectory() {
        return m_directory;
    }

private:
    std::vector<Texture> m_textures;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::vector<glm::mat4> boneTrans;
    std::map<std::string, uint> m_boneMapping;
    std::map<std::string, glm::mat4> m_boneTransfromations;
    std::map<std::string, glm::mat4> m_nodeTransformations;
    std::map<std::string, std::shared_ptr<Mesh>> m_meshMapping;

    Assimp::Importer m_importer;
    const aiScene* m_scene;
    const aiNode* m_rootNode;
    const aiAnimation* m_animation;

    float m_ticksPerSecond;
    float m_duration;

    std::shared_ptr<NodeData> m_rootNodeData;

    std::string m_directory;

    void loadModel();
    void processNode(const aiNode* node, const aiScene *scene, const glm::mat4& parentMatrix, std::shared_ptr<NodeData> parentNodeData);
    void processMesh(const aiMesh* mesh, const aiScene *scene, MeshData& meshData);
    void processBone(const aiMesh* mesh, std::shared_ptr<Mesh> p_mesh, MeshData* meshData);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    glm::vec3 convertScale(aiMatrix4x4 t_matrix);
    glm::mat4 convertTrans(aiMatrix4x4 t_matrix, glm::vec3 scale);
    glm::mat4 convertMatrix(aiMatrix4x4 t_matrix, bool needTransPos = true);
    glm::mat4 convertMatrix(aiMatrix3x3 t_matrix);

    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const NodeAnimation& nodeAnimation);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const NodeAnimation& nodeAnimation);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const NodeAnimation& nodeAnimation);
    uint FindRotation(float AnimationTime, const NodeAnimation& nodeAnimation);
    uint FindPosition(float AnimationTime, const NodeAnimation& nodeAnimation);
    uint FindScaling(float AnimationTime, const NodeAnimation& nodeAnimation);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

    void BoneTransform(std::shared_ptr<NodeData> nodeData, float animationTime,
                    const glm::mat4& parentMatrix, const std::string& parentName);
};