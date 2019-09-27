#include "Model.h"

Model::Model(const std::string& name, const Camera::Ptr& camera,
        const ObjectAttribute& attribute, const std::string& path, bool useLight) 
        : Object(name, camera, attribute, useLight), m_directory(path) {
}

void Model::init() {
    loadModel();
    initMeshes();
}

void Model::loadModel() {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_directory, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    auto root = scene->mRootNode;
 
    for(unsigned int i = 0; i < root->mNumChildren; i++) {
        processNode(root->mChildren[i], scene);
    }
}

void Model::initMeshes() {

    for (unsigned int i = 0; i < m_meshdatas.size(); i++)
    {
        auto meshData = m_meshdatas[i];
        auto mesh = std::make_shared<Mesh>(meshData.name, m_camera, m_attribute);
        mesh->setData(meshData);
        mesh->init();
        m_meshes.push_back(mesh);
    }
    
}

void Model::render() {

    for (auto mesh : m_meshes)
    {
        mesh->render();
    }
}

void Model::processNode(const aiNode* node, const aiScene *scene) {

    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(const aiMesh* mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]) {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    auto name = std::string(mesh->mName.C_Str());
    m_meshdatas.push_back({name, vertices, indices, textures});
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;

        for(unsigned int j = 0; j < textures.size(); j++)
        {
            if(std::strcmp(textures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures[j]);
                skip = true;
                break;
            }
        }

        if(!skip)
        {
            Texture texture;
            auto filename = m_directory.substr(0, m_directory.find_last_of('/')) + '/' + std::string(str.C_Str());
            texture.id = loadTexture(filename.c_str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures.push_back(texture);
        }
    }
    return textures;
}