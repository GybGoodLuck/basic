cmake_minimum_required(VERSION 3.0.0)
project(basic VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(PYTHON_DIRECTORY "${CMAKE_SOURCE_DIR}/config/config.py")
execute_process(COMMAND python ${PYTHON_DIRECTORY})

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
# note that the order is important for setting the libs
# use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread ${ASSIMP_LIBRARY})
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

set(NAME basic)

set(EXECUTABLE_SOURCE
    src/characters/Characters.cpp
    src/window/Window.cpp
    src/camera/Camera.cpp
    src/object/Object.cpp
    src/object/Plane.cpp
    src/object/Font.cpp
    src/vertices/Vertices.cpp
    src/GLUtils.cpp
    src/main.cpp
)

add_executable(${NAME} ${EXECUTABLE_SOURCE})
target_link_libraries(${NAME} ${LIBS})
FIND_LIBRARY(FREETYPE libfreetype.so.6.17.1 lib/)
target_link_libraries(${NAME} ${FREETYPE})

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/config)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
