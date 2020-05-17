#include "Node.h"

glm::mat4 Node::getTransByAnimTime(float animTime) {

    if (hasAnimation) {
        glm::mat4 animatrix = glm::mat4(1.0);
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        calcInterpolatedScaling(Scaling, animTime, m_animation);
    
        animatrix = glm::scale(animatrix, glm::vec3(Scaling.x, Scaling.y, Scaling.z));
    
        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        calcInterpolatedPosition(Translation, animTime, m_animation);
        animatrix = glm::translate(animatrix, glm::vec3(Translation.x, Translation.y, Translation.z));
    
        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        calcInterpolatedRotation(RotationQ, animTime, m_animation);
        glm::quat qua;
        qua.x = RotationQ.x;
        qua.y = RotationQ.y;
        qua.z = RotationQ.z;
        qua.w = RotationQ.w;
        animatrix = animatrix * glm::mat4_cast(qua);

        // Combine the above transformations
        return glm::transpose(animatrix);
    }

    return m_transformation;
}

void Node::calcInterpolatedPosition(aiVector3D& Out, float animationTime, const NodeAnimation& nodeAnimation)
{
    if (nodeAnimation.positionKeys.size() == 1) {
        Out.x = nodeAnimation.positionKeys[0].value.x;
        Out.y = nodeAnimation.positionKeys[0].value.y;
        Out.z = nodeAnimation.positionKeys[0].value.z;
        return;
    }

    uint PositionIndex = findPosition(animationTime, nodeAnimation);
    uint NextPositionIndex = (PositionIndex + 1);

    float DeltaTime = (float)(nodeAnimation.positionKeys[NextPositionIndex].time - nodeAnimation.positionKeys[PositionIndex].time);
    float Factor = (animationTime - (float)nodeAnimation.positionKeys[PositionIndex].time) / DeltaTime;

    glm::vec3 start = nodeAnimation.positionKeys[PositionIndex].value;
    glm::vec3 end = nodeAnimation.positionKeys[NextPositionIndex].value;

    glm::vec3 delta = end - start;
    auto out = start + Factor * delta;
    Out.x = out.x;
    Out.y = out.y;
    Out.z = out.z;
}

void Node::calcInterpolatedRotation(aiQuaternion& Out, float animationTime, const NodeAnimation& nodeAnimation)
{
	// we need at least two values to interpolate...
    if (nodeAnimation.rotationKeys.size() == 1) {
        Out.x = nodeAnimation.rotationKeys[0].value.x;
        Out.y = nodeAnimation.rotationKeys[0].value.y;
        Out.z = nodeAnimation.rotationKeys[0].value.z;
        Out.w = nodeAnimation.rotationKeys[0].value.w;
        return;
    }
    
    uint RotationIndex = findRotation(animationTime, nodeAnimation);
    uint NextRotationIndex = (RotationIndex + 1);

    float DeltaTime = (float)(nodeAnimation.rotationKeys[NextRotationIndex].time - nodeAnimation.rotationKeys[RotationIndex].time);
    float Factor = (animationTime - (float)nodeAnimation.rotationKeys[RotationIndex].time) / DeltaTime;

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

void Node::calcInterpolatedScaling(aiVector3D& Out, float animationTime, const NodeAnimation& nodeAnimation)
{
    if (nodeAnimation.scaleKeys.size() == 1) {
        Out.x = nodeAnimation.scaleKeys[0].value.x;
        Out.y = nodeAnimation.scaleKeys[0].value.y;
        Out.z = nodeAnimation.scaleKeys[0].value.z;
        return;
    }

    uint ScalingIndex = findScaling(animationTime, nodeAnimation);
    uint NextScalingIndex = (ScalingIndex + 1);

    float DeltaTime = (float)(nodeAnimation.scaleKeys[NextScalingIndex].time - nodeAnimation.scaleKeys[ScalingIndex].time);
    float Factor = (animationTime - (float)nodeAnimation.scaleKeys[ScalingIndex].time) / DeltaTime;

    auto start = nodeAnimation.scaleKeys[ScalingIndex].value;
    auto end   = nodeAnimation.scaleKeys[NextScalingIndex].value;
    auto delta = end - start;
    auto out = start + Factor * delta;

    Out.x = out.x;
    Out.y = out.y;
    Out.z = out.z;
}

uint Node::findRotation(float animationTime, const NodeAnimation& nodeAnimation)
{
    for (uint i = 0 ; i < nodeAnimation.rotationKeys.size() - 1 ; i++) {
        if (animationTime < (float)nodeAnimation.rotationKeys[i + 1].time) {
            return i;
        }
    }
    
    return 0;
}

uint Node::findPosition(float animationTime, const NodeAnimation& nodeAnimation) {

    for (uint i = 0 ; i < nodeAnimation.positionKeys.size() - 1 ; i++) {
        if (animationTime < (float)nodeAnimation.positionKeys[i + 1].time) {
            return i;
        }
    }

    return 0;
}

uint Node::findScaling(float animationTime, const NodeAnimation& nodeAnimation)
{
    for (uint i = 0 ; i < nodeAnimation.scaleKeys.size() - 1 ; i++) {
        if (animationTime < (float)nodeAnimation.scaleKeys[i + 1].time) {
            return i;
        }
    }

    return 0;
}

Node::Ptr Node::getChildrenByName(const std::string& name) {

    for (auto child : m_children) {
        if (child->m_name == name) {
            return child;
        }
    }

    for (auto child : m_children) {
        auto ans = child->getChildrenByName(name);

        if (ans) {
            return ans;
        }
    }
    return nullptr;
}