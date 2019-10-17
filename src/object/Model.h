#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Common.h"
#include "../GLUtils.h"

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

    std::vector<MeshData> getMeshDatas() {
        return m_meshdatas;
    }

    std::string getDirectory() {
        return m_directory;
    }

private:
    std::vector<Texture> m_textures;
    std::vector<MeshData> m_meshdatas;
    std::vector<Mesh::Ptr> m_meshes;

    std::string m_directory;

    void loadModel();
    void initMeshes();
    void processNode(const aiNode* node, const aiScene *scene);
    void processMesh(const aiMesh* mesh, const aiScene *scene, MeshData& meshData);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};