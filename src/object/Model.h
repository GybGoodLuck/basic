#pragma once

#include "../Common.h"
#include "../GLUtils.h"
#include "Skeleton.h"

#define MAX_FLOAT 100000000.0f
#define MIN_FLOAT -100000000.0f
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
    std::vector<Mesh::Ptr> m_meshes;

    Assimp::Importer m_importer;
    const aiScene* m_scene;
    const aiAnimation* m_animation;

    float m_ticksPerSecond;
    float m_duration;

    Skeleton::Ptr m_skeleton;
    Node::Ptr rootNode;

    std::string m_directory;

    float m_box[3];
    float m_sides[6] = {MAX_FLOAT, MIN_FLOAT, MAX_FLOAT, MIN_FLOAT, MAX_FLOAT, MIN_FLOAT};

    void loadModel();
    void processNode(const aiNode* node, const Node::Ptr& parentNode);
    void processNodeMesh(const aiNode* pNode, const Node::Ptr& parentNode);
    void processMesh(const aiMesh* mesh, const Node::Ptr& node);
    void processBone(const aiMesh* mesh, std::shared_ptr<Mesh> p_mesh, MeshData* meshData);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
    void BoneTransform(const Node::Ptr& node, float animationTime);

    glm::mat4 convertMatrix(aiMatrix4x4 t_matrix, bool needTransPos = true);
};