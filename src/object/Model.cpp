#include <iostream>

#include "Model.h"

Model::Model(const std::string& name, const Camera::Ptr& camera,
        const ObjectAttribute& attribute, const std::string& path, bool useLight) 
        : Object(name, camera, attribute, useLight), m_directory(path) {
    m_scene = NULL;
    m_rootNode = NULL;
    m_animation = NULL;
}

void Model::init() {

    m_scene = m_importer.ReadFile(m_directory, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << m_importer.GetErrorString() << std::endl;
        return;
    }

    loadModel();

    for (unsigned int i; i < 6; i++) {
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
    m_rootNode = m_scene->mRootNode;

    std::string animName(m_scene->mAnimations[0]->mName.C_Str());
    std::cout << animName << " : " << m_scene->mNumAnimations <<std::endl;
    if (m_scene->HasAnimations())
    {
        m_animation = m_scene->mAnimations[0];
        m_ticksPerSecond = (float)(m_animation->mTicksPerSecond != 0 ? m_animation->mTicksPerSecond : 25.0f);
        m_duration = (float)m_animation->mDuration;
    }

    m_rootNodeData = std::make_shared<NodeData>();
    m_rootNodeData->name = std::string(m_rootNode->mName.C_Str());
    m_rootNodeData->transformation = convertMatrix(m_rootNode->mTransformation, false);

    for(unsigned int i = 0; i < m_rootNode->mNumChildren; i++) {
        processNode(m_rootNode->mChildren[i], m_scene, glm::mat4(1.0), m_rootNodeData);
    }
}

void Model::render() {
    float timeInTicks = m_runningTime * m_ticksPerSecond;
    float animationTime = fmod(timeInTicks, m_duration - 0.4);

    BoneTransform(m_rootNodeData, animationTime, glm::mat4(1.0), std::string(m_rootNode->mName.C_Str()));

    for (auto mesh : m_meshes)
    {
        mesh->setScale(m_attribute.scale);
        mesh->setPos(m_attribute.pos);
        mesh->setQua(m_attribute.quat);
        mesh->update();
        mesh->render();
    }
}

void Model::processNode(const aiNode* node, const aiScene *scene,
         const glm::mat4& parentMatrix, std::shared_ptr<NodeData> parentNodeData) {

    std::shared_ptr<NodeData> nodeData = std::make_shared<NodeData>();
    
    auto name = std::string(node->mName.C_Str());
    auto t_matrix = convertMatrix(node->mTransformation, false);

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

            nodeData->nodeAnimation = nodeAnimation;
            nodeData->hasNodeAnimation = true;
        } else {
            nodeData->hasNodeAnimation = false;
        }
    } else {
        nodeData->hasNodeAnimation = false;
    }

    nodeData->name = name;
    nodeData->transformation = t_matrix;

    parentNodeData->children.push_back(nodeData);

    t_matrix = t_matrix * parentMatrix;

    auto nodeTransformation = glm::inverse(convertMatrix(scene->mRootNode->mTransformation, false)) * t_matrix;

    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, name, nodeTransformation);
    }
        
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, t_matrix, nodeData);
    }
}

void Model::processMesh(const aiMesh* mesh, const aiScene *scene, const std::string& nodeName, const glm::mat4& nodeTransformation) {

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

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

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
    meshData.name = nodeName;
    meshData.transformation = nodeTransformation;

    meshData.color = meshColor;
    meshData.vertices = vertices;
    meshData.indices = indices;
    meshData.textures = textures;

    auto p_mesh = std::make_shared<Mesh>(nodeName, m_camera, m_attribute);
    m_meshMapping[nodeName].push_back(p_mesh);
    meshData.boneDatas.resize(mesh->mNumVertices);

    processBone(mesh, p_mesh, &meshData);
    p_mesh->setData(meshData);
    m_meshes.push_back(p_mesh);
    p_mesh->init();
}

void Model::processBone(const aiMesh* mesh, std::shared_ptr<Mesh> p_mesh, MeshData* meshData) {
 
    meshData->boneOffsets.resize(mesh->mNumBones);

    for (uint i = 0 ; i < mesh->mNumBones; i++) {
        auto pBone = mesh->mBones[i];

        std::string name(pBone->mName.C_Str());

        p_mesh->addBone(name);

        auto boneId = p_mesh->findBone(name);

        if (boneId != -1) {
            meshData->boneOffsets[boneId] = convertMatrix(pBone->mOffsetMatrix, false);
            p_mesh->addBoneOffsets(boneId, convertMatrix(pBone->mOffsetMatrix, false));
            m_meshMapping[name].push_back(p_mesh);
        } else {
            std::cout << "error : invaild bone id" << std::endl;
        }

        for (uint j = 0 ; j < pBone->mNumWeights ; j++) {
            uint vertexId = pBone->mWeights[j].mVertexId;
            float weight  = pBone->mWeights[j].mWeight;
            for (uint k = 0 ; k < 4 ; k++) {
                if (meshData->boneDatas[vertexId].weights[k] == 0.f) {
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

glm::vec3 Model::convertScale(aiMatrix4x4 t_matrix) {
    // scale
    auto t_scaling_x = aiVector3D(t_matrix.a1, t_matrix.a2, t_matrix.a3).Length();
    auto t_scaling_y = aiVector3D(t_matrix.b1, t_matrix.b2, t_matrix.b3).Length();
    auto t_scaling_z = aiVector3D(t_matrix.c1, t_matrix.c2, t_matrix.c3).Length();
    glm::vec3 scale = {t_scaling_x, t_scaling_y, t_scaling_z};

    return scale;
}

glm::mat4 Model::convertTrans(aiMatrix4x4 t_matrix, glm::vec3 scale) {

    // quat
    t_matrix.Transpose();

    glm::mat4 trans = {
        t_matrix.a1 / scale.x, t_matrix.a2 / scale.x, t_matrix.a3 / scale.x, t_matrix.a4,
        t_matrix.b1 / scale.y, t_matrix.b2 / scale.y, t_matrix.b3 / scale.y, t_matrix.b4,
        t_matrix.c1 / scale.z, t_matrix.c2 / scale.z, t_matrix.c3 / scale.z, t_matrix.c4,
        t_matrix.d1, t_matrix.d2, t_matrix.d3, t_matrix.d4
    };

    return trans;
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

glm::mat4 Model::convertMatrix(aiMatrix3x3 t_matrix) {
    glm::mat4 m;
    m[0][0] = t_matrix.a1; m[0][1] = t_matrix.a2; m[0][2] = t_matrix.a3; m[0][3] = 0.0f;
    m[1][0] = t_matrix.b1; m[1][1] = t_matrix.b2; m[1][2] = t_matrix.b3; m[1][3] = 0.0f;
    m[2][0] = t_matrix.c1; m[2][1] = t_matrix.c2; m[2][2] = t_matrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    return m;
}

void Model::BoneTransform(std::shared_ptr<NodeData> nodeData, float animationTime,
                    const glm::mat4& parentMatrix, const std::string& parentName) {

    auto name = nodeData->name;
    auto t_matrix = nodeData->transformation;

    if (nodeData->hasNodeAnimation) {

       auto nodeAnimation = nodeData->nodeAnimation;  

       glm::mat4 animatrix = glm::mat4(1.0);

        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, animationTime, nodeAnimation);

        animatrix = glm::scale(animatrix, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, animationTime, nodeAnimation);

        animatrix = glm::translate(animatrix, glm::vec3(Translation.x, Translation.y, Translation.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, animationTime, nodeAnimation);
        glm::quat qua;
        qua.x = RotationQ.x;
        qua.y = RotationQ.y;
        qua.z = RotationQ.z;
        qua.w = RotationQ.w;
        animatrix = animatrix * glm::mat4_cast(qua);

        // Combine the above transformations
        t_matrix = glm::transpose(animatrix);
    }

    t_matrix = t_matrix * parentMatrix;

    auto itMeshes = m_meshMapping.find(name);

    if (itMeshes != m_meshMapping.end()) {
        auto meshes = itMeshes->second;

        for (unsigned int i = 0; i < meshes.size(); i++) {
            auto globalTrans = glm::inverse(m_rootNodeData->transformation);
            auto bt = t_matrix * globalTrans;
            auto mesh = meshes[i];
            auto boneID = mesh->findBone(name);

            if (boneID != -1) {
                auto boneOffset = mesh->getBoneOffset(boneID);
                mesh->setBoneTransformation(boneID, boneOffset * bt);
            }

            mesh->setTransformation(bt);
        }
    }

    for(auto child : nodeData->children) {
        BoneTransform(child, animationTime, t_matrix, name);
    }
}

void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const NodeAnimation& nodeAnimation)
{
    if (nodeAnimation.positionKeys.size() == 1) {
        Out.x = nodeAnimation.positionKeys[0].value.x;
        Out.y = nodeAnimation.positionKeys[0].value.y;
        Out.z = nodeAnimation.positionKeys[0].value.z;
        return;
    }

    uint PositionIndex = FindPosition(AnimationTime, nodeAnimation);
    uint NextPositionIndex = (PositionIndex + 1);

    float DeltaTime = (float)(nodeAnimation.positionKeys[NextPositionIndex].time - nodeAnimation.positionKeys[PositionIndex].time);
    float Factor = (AnimationTime - (float)nodeAnimation.positionKeys[PositionIndex].time) / DeltaTime;

    glm::vec3 start = nodeAnimation.positionKeys[PositionIndex].value;
    glm::vec3 end = nodeAnimation.positionKeys[NextPositionIndex].value;

    glm::vec3 delta = end - start;
    auto out = start + Factor * delta;
    Out.x = out.x;
    Out.y = out.y;
    Out.z = out.z;
}

void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const NodeAnimation& nodeAnimation)
{
	// we need at least two values to interpolate...
    if (nodeAnimation.rotationKeys.size() == 1) {
        Out.x = nodeAnimation.rotationKeys[0].value.x;
        Out.y = nodeAnimation.rotationKeys[0].value.y;
        Out.z = nodeAnimation.rotationKeys[0].value.z;
        Out.w = nodeAnimation.rotationKeys[0].value.w;
        return;
    }
    
    uint RotationIndex = FindRotation(AnimationTime, nodeAnimation);
    uint NextRotationIndex = (RotationIndex + 1);

    float DeltaTime = (float)(nodeAnimation.rotationKeys[NextRotationIndex].time - nodeAnimation.rotationKeys[RotationIndex].time);
    float Factor = (AnimationTime - (float)nodeAnimation.rotationKeys[RotationIndex].time) / DeltaTime;

    auto startRotationQ = nodeAnimation.rotationKeys[RotationIndex].value;
    auto endRotationQ   = nodeAnimation.rotationKeys[NextRotationIndex].value;

    aiQuaternion StartRotationQ, EndRotationQ;
    StartRotationQ.x = startRotationQ.x;
    StartRotationQ.y = startRotationQ.y;
    StartRotationQ.z = startRotationQ.z;
    StartRotationQ.w = startRotationQ.w;

    EndRotationQ.x = endRotationQ.x;
    EndRotationQ.y = endRotationQ.y;
    EndRotationQ.z = endRotationQ.z;
    EndRotationQ.w = endRotationQ.w;

    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const NodeAnimation& nodeAnimation)
{
    if (nodeAnimation.scaleKeys.size() == 1) {
        Out.x = nodeAnimation.scaleKeys[0].value.x;
        Out.y = nodeAnimation.scaleKeys[0].value.y;
        Out.z = nodeAnimation.scaleKeys[0].value.z;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, nodeAnimation);
    uint NextScalingIndex = (ScalingIndex + 1);

    float DeltaTime = (float)(nodeAnimation.scaleKeys[NextScalingIndex].time - nodeAnimation.scaleKeys[ScalingIndex].time);
    float Factor = (AnimationTime - (float)nodeAnimation.scaleKeys[ScalingIndex].time) / DeltaTime;

    auto start = nodeAnimation.scaleKeys[ScalingIndex].value;
    auto end   = nodeAnimation.scaleKeys[NextScalingIndex].value;
    auto delta = end - start;
    auto out = start + Factor * delta;

    Out.x = out.x;
    Out.y = out.y;
    Out.z = out.z;
}

uint Model::FindRotation(float AnimationTime, const NodeAnimation& nodeAnimation)
{
    for (uint i = 0 ; i < nodeAnimation.rotationKeys.size() - 1 ; i++) {
        if (AnimationTime < (float)nodeAnimation.rotationKeys[i + 1].time) {
            return i;
        }
    }
    
    return 0;
}

uint Model::FindPosition(float AnimationTime, const NodeAnimation& nodeAnimation) {

    for (uint i = 0 ; i < nodeAnimation.positionKeys.size() - 1 ; i++) {
        if (AnimationTime < (float)nodeAnimation.positionKeys[i + 1].time) {
            return i;
        }
    }

    return 0;
}

uint Model::FindScaling(float AnimationTime, const NodeAnimation& nodeAnimation)
{
    for (uint i = 0 ; i < nodeAnimation.scaleKeys.size() - 1 ; i++) {
        if (AnimationTime < (float)nodeAnimation.scaleKeys[i + 1].time) {
            return i;
        }
    }

    return 0;
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