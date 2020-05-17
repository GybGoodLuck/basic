#include <iostream>

#include "Model.h"

Model::Model(const std::string& name, const Camera::Ptr& camera,
        const ObjectAttribute& attribute, const std::string& path, bool useLight) 
        : Object(name, camera, attribute, useLight), m_directory(path) {
    m_scene = NULL;
    m_animation = NULL;
}

void Model::init() {

    m_scene = m_importer.ReadFile(m_directory, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << m_importer.GetErrorString() << std::endl;
        return;
    }

    m_skeleton = std::make_shared<Skeleton>();
    loadModel();

    for (unsigned int i = 0; i < 6; i++) {
        std::cout << m_sides[i] << " ";
    }
    std::cout << std::endl;

    m_box[0] = std::abs(m_sides[0] - m_sides[1]);
    m_box[1] = std::abs(m_sides[2] - m_sides[3]);
    m_box[2] = std::abs(m_sides[4] - m_sides[5]);

    float maxLength = std::max(m_box[0], m_box[1]);
    maxLength = std::max(maxLength, m_box[2]);

    float scale = 1.f / maxLength;
    std::cout << "scale ： " << scale << std::endl;
    m_attribute.scale = glm::vec3(scale * m_attribute.scale.x,
            scale * m_attribute.scale.y, scale * m_attribute.scale.z);
}

void Model::loadModel() {
    auto p_rootNode = m_scene->mRootNode;

    if (m_scene->HasAnimations())
    {
        std::string animName(m_scene->mAnimations[0]->mName.C_Str());
        std::cout << animName << " : " << m_scene->mNumAnimations <<std::endl;
        m_animation = m_scene->mAnimations[0];
        m_ticksPerSecond = (float)(m_animation->mTicksPerSecond != 0 ? m_animation->mTicksPerSecond : 25.0f);
        m_duration = (float)m_animation->mDuration;
    }

    auto rootTransformation = convertMatrix(p_rootNode->mTransformation, false);
    rootNode = std::make_shared<Node>(std::string(p_rootNode->mName.C_Str()), rootTransformation);
    rootNode->setWorldTransformation(rootTransformation);

    for(unsigned int i = 0; i < p_rootNode->mNumChildren; i++) {
        processNode(p_rootNode->mChildren[i], rootNode);
    }

    processNodeMesh(p_rootNode, rootNode);
}

void Model::render() {
    float timeInTicks = m_runningTime * m_ticksPerSecond;
    float animationTime = fmod(timeInTicks, m_duration - 0.4);

    for (auto child : rootNode->getChildren()) {
        BoneTransform(child, animationTime);
    }

    for (auto mesh : m_meshes)
    {
        mesh->setScale(m_attribute.scale);
        mesh->setPos(m_attribute.pos);
        mesh->setQua(m_attribute.quat);
        mesh->setBoneTransformations(m_skeleton->getBoneTransforms());
        mesh->update();
        mesh->render();
    }
}

void Model::processNode(const aiNode* node, const Node::Ptr& parentNode) {
    
    auto name = std::string(node->mName.C_Str());
    auto t_matrix = convertMatrix(node->mTransformation, false);

    Node::Ptr child = std::make_shared<Node>(name, t_matrix);

    if (m_scene->HasAnimations()) {

        auto pNodeAnim = FindNodeAnim(m_animation, name);

        if (pNodeAnim != NULL) {
            NodeAnimation nodeAnimation;
            nodeAnimation.scaleKeys.resize(pNodeAnim->mNumScalingKeys);
            nodeAnimation.positionKeys.resize(pNodeAnim->mNumPositionKeys);
            nodeAnimation.rotationKeys.resize(pNodeAnim->mNumRotationKeys);

            for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys; i++) {
                nodeAnimation.scaleKeys[i].time = pNodeAnim->mScalingKeys[i].mTime;
                nodeAnimation.scaleKeys[i].value.x = pNodeAnim->mScalingKeys[i].mValue.x;
                nodeAnimation.scaleKeys[i].value.y = pNodeAnim->mScalingKeys[i].mValue.y;
                nodeAnimation.scaleKeys[i].value.z = pNodeAnim->mScalingKeys[i].mValue.z;
            }

            for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys; i++) {
                nodeAnimation.positionKeys[i].time = pNodeAnim->mPositionKeys[i].mTime;
                nodeAnimation.positionKeys[i].value.x = pNodeAnim->mPositionKeys[i].mValue.x;
                nodeAnimation.positionKeys[i].value.y = pNodeAnim->mPositionKeys[i].mValue.y;
                nodeAnimation.positionKeys[i].value.z = pNodeAnim->mPositionKeys[i].mValue.z;
            }

            for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys; i++) {
                nodeAnimation.rotationKeys[i].time = pNodeAnim->mRotationKeys[i].mTime;
                nodeAnimation.rotationKeys[i].value.x = pNodeAnim->mRotationKeys[i].mValue.x;
                nodeAnimation.rotationKeys[i].value.y = pNodeAnim->mRotationKeys[i].mValue.y;
                nodeAnimation.rotationKeys[i].value.z = pNodeAnim->mRotationKeys[i].mValue.z;
                nodeAnimation.rotationKeys[i].value.w = pNodeAnim->mRotationKeys[i].mValue.w;
            }

            child->setNodeAnimation(nodeAnimation);
        }
    }

    auto worldTransformation = t_matrix * parentNode->getWorldTransformation();
    child->setWorldTransformation(worldTransformation);

    child->setParent(parentNode);
    parentNode->addChild(child);

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], child);
    }
}

void Model::processNodeMesh(const aiNode* parentPNode, const Node::Ptr& parentNode) {

    for(unsigned int i = 0; i < parentPNode->mNumChildren; i++) {
        auto pNode = parentPNode->mChildren[i];
        auto node = parentNode->getChildren()[i];

        for(unsigned int i = 0; i < pNode->mNumMeshes; i++)
        {
            auto mesh = m_scene->mMeshes[pNode->mMeshes[i]];
            processMesh(mesh, node);
        }

        processNodeMesh(pNode, node);
    }
}

void Model::processMesh(const aiMesh* mesh, const Node::Ptr& node) {

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

        m_sides[0] = std::min(m_sides[0], vector.x);
        m_sides[1] = std::max(m_sides[1], vector.x);

        m_sides[2] = std::min(m_sides[2], vector.y);
        m_sides[3] = std::max(m_sides[3], vector.y);

        m_sides[4] = std::min(m_sides[4], vector.y);
        m_sides[5] = std::max(m_sides[5], vector.y);

        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->HasTextureCoords(0)) {
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

    aiMaterial* material = m_scene->mMaterials[mesh->mMaterialIndex]; 

    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    auto meshColor = glm::vec3(color.r, color.g, color.b);

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    MeshData meshData;
    meshData.name = std::string(mesh->mName.C_Str());

    meshData.color = meshColor;
    meshData.vertices = vertices;
    meshData.indices = indices;
    meshData.textures = textures;

    auto p_mesh = std::make_shared<Mesh>(meshData.name, m_camera, m_attribute);
    meshData.boneDatas.resize(mesh->mNumVertices);

    processBone(mesh, p_mesh, &meshData);
    p_mesh->setData(meshData);
    p_mesh->init();
    m_meshes.push_back(p_mesh);
    node->addMesh(p_mesh);
}

void Model::processBone(const aiMesh* mesh, std::shared_ptr<Mesh> p_mesh, MeshData* meshData) {

    for (uint i = 0 ; i < mesh->mNumBones; i++) {
        auto pBone = mesh->mBones[i];
        std::string name(pBone->mName.C_Str());
        auto bone = rootNode->getChildrenByName(name);
        m_scene->mRootNode->FindNode(pBone->mName);
        m_skeleton->addBone(bone);
        auto boneId = bone->getBoneId();
        auto boneOffset = convertMatrix(pBone->mOffsetMatrix, false);
        p_mesh->setBoneOffset(boneId, boneOffset);

        for (uint j = 0 ; j < pBone->mNumWeights ; j++) {
            uint vertexId = pBone->mWeights[j].mVertexId;
            float weight  = pBone->mWeights[j].mWeight;

            for (uint k = 0 ; k < 4 ; k++) {

                if (k < 4 && meshData->boneDatas[vertexId].weights[k] == 0.f) {
                    meshData->boneDatas[vertexId].ids[k] = boneId;
                    meshData->boneDatas[vertexId].weights[k] = weight;
                    break;
                }
            }
        }
    }
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

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        auto name = std::string(pNodeAnim->mNodeName.data);
        if (name == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}

void Model::BoneTransform(const Node::Ptr& node, float animationTime) {

    auto name = node->getName();
    auto t_matrix = node->getTransformation();

    if (node->hasNodeAnimation()) {
        t_matrix = node->getTransByAnimTime(animationTime);
    }

    t_matrix = t_matrix * node->getParent()->getWorldTransformation();
    node->setWorldTransformation(t_matrix);

    for(auto child : node->getChildren()) {
        BoneTransform(child, animationTime);
    }
}

glm::mat4 Model::convertMatrix(aiMatrix4x4 t_matrix, bool needTransPos) {

    if (needTransPos) t_matrix.Transpose();

    glm::mat4 m;
    m[0][0] = t_matrix.a1; m[0][1] = t_matrix.a2; m[0][2] = t_matrix.a3; m[0][3] = t_matrix.a4;
    m[1][0] = t_matrix.b1; m[1][1] = t_matrix.b2; m[1][2] = t_matrix.b3; m[1][3] = t_matrix.b4;
    m[2][0] = t_matrix.c1; m[2][1] = t_matrix.c2; m[2][2] = t_matrix.c3; m[2][3] = t_matrix.c4;
    m[3][0] = t_matrix.d1; m[3][1] = t_matrix.d2; m[3][2] = t_matrix.d3; m[3][3] = t_matrix.d4;

    return m;
}