//
// Created by yuanbin on 19-8-12.
//

#include <functional>

#include "Common.h"
#include "GLUtils.h"

GLint createProgram(const char* vShader, const char* fShader) {
    auto vertexShader = loadShader(GL_VERTEX_SHADER, vShader);
    if (!vertexShader) {
        return 0;
    }

    auto pixelShader = loadShader(GL_FRAGMENT_SHADER, fShader);
    if (!pixelShader) {
        return 0;
    }

    auto program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");
    }

    return program;
}

GLint loadShader(GLenum shaderType, const char *pSource) {
    auto shader = glCreateShader(shaderType);

    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, "SHADER");
    }

    return shader;
}

GLuint loadTexture(const char* path) {

    GLuint texture;

    typedef std::unique_ptr<unsigned char, std::function<void(unsigned char*)>>
            stbi_ptr;

    int w, h, n;
    stbi_set_flip_vertically_on_load(false);
    stbi_ptr data(stbi_load(path, &w, &h, &n, 0),
                  [](unsigned char* data) {
                      if (data) stbi_image_free(data);
                  });

    std::cout << "loadTexture w : " <<  w  << " h : " <<  h << std::endl;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //线性滤图
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum internalFormat;
    GLenum dataFormat;

    switch (n) {
        case 1:
            internalFormat = dataFormat = GL_RED;
            break;
        case 3:
            internalFormat = GammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
            break;
        default:
            break;
    }

    //生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, dataFormat, GL_UNSIGNED_BYTE, data.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    std::cout << "loadTexture texture : " << texture << std::endl;

    return texture;
}

void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}