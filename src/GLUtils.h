//
// Created by yuanbin on 19-8-12.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <memory>
#include <iostream>

GLint createProgram(const char* vShader, const char* fShader);

GLint loadShader(GLenum shaderType, const char* pSource);

GLuint loadTexture(const char* path);
