#pragma once

#include "../camera/Camera.h"
#include "../light/LightManager.h"
#include "../vertices/Vertices.h"
#include "../shader/Shader.h"
#include "../GLUtils.h"

class Object {

public:
    using Ptr = std::shared_ptr<Object>;

    Object(const std::string& name, const Camera::Ptr& camera, 
        const ObjectAttribute& attribute, bool useLight = false, 
        bool useReflect = false);
    virtual ~Object() {};

    virtual ObjectType type() = 0;
    virtual void render() = 0;
    virtual void init() = 0;

    virtual void update();
    virtual void updateBones();

    std::string getName() const {
        return m_name;
    };

    void setRunningTime(float runningTime) {
        m_runningTime = runningTime;
    }

    float getRunnintTime() {
        return m_runningTime;
    }

    void setPos(const glm::vec3& pos) {
        m_attribute.pos = pos;
    };

    glm::vec3 getPos() const {
        return m_attribute.pos;
    };

    void setQua(const glm::quat& qua) {
        m_attribute.quat = qua;
    };

    glm::quat getQua() const {
        return m_attribute.quat;
    };

    void setColor(const glm::vec3& color) {
        m_attribute.color = color;
    };

    glm::vec3 getColor() const {
        return m_attribute.color;
    };

    void setAlpha(float alpha) {
        m_attribute.alpha = alpha;
    };

    float getAlpha() const {
        return m_attribute.alpha;
    }

    void setTextureID(GLuint textureID) {
        m_attribute.textureID = textureID;
    };

    GLuint getTextureID() {
        return m_attribute.textureID;
    };

    glm::vec3 getScale() const {
        return m_attribute.scale;
    }

    void setScale(const glm::vec3 scale) {
        m_attribute.scale = scale;
    }

protected:
    std::string m_name;
    float m_runningTime;

    Camera::Ptr m_camera;
    ObjectAttribute m_attribute;

    bool m_useLight = false;
    bool m_useReflect = false;
    bool m_blinn = true;

    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    BBO m_bbo;
    int m_indexCount;

    GLuint m_program;

    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_model;

    GLint projection;
    GLint view;
    GLint model;
    GLint global;
    GLint globals;
    GLint color;

    GLint use_light;
    GLint use_reflect;
    GLint light_size;
    GLint light_pos;
    GLint light_color;
    GLint blinn;
    GLint gamma;

    GLint camera_pos;

    GLint bones;

    void getUniformLocation();
    void updateLocation();
    void updateCamera();
    void updateLight();
};