#include <iostream>
#include <memory>

#include <Config.h>

#include "window/Window.h"
#include "controller/Controller.h"

#include "object/Plane.h"
#include "object/Font.h"
#include "object/Cube.h"

#include "Common.h"

using namespace std;

int main(int, char**) {

    auto camera =  make_shared<Camera>();
    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT, camera);
    Controller::getInstance()->setCamera(camera);

    ObjectAttribute groundAttribute; 
    std::string path = "image/car.jpg";
    path = RES_PATH + path;
    groundAttribute.textureID = loadTexture(path.c_str());
    groundAttribute.pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 axis = {1.0f, 0.0f, 0.0f};
    groundAttribute.quat = glm::angleAxis(glm::radians(90.0f), axis) * groundAttribute.quat;
    groundAttribute.scale = {14.2f, 1.0f, 8.0f};
    auto plane = make_shared<Plane>("ground", camera, groundAttribute);

    ObjectAttribute planeAttribute;
    std::string yzqPath = "image/yzq.jpg";
    yzqPath = RES_PATH + yzqPath;
    planeAttribute.textureID = loadTexture(yzqPath.c_str());
    planeAttribute.pos = {0.0f, 1.0f, 2.0f};
    auto yzqPlane = make_shared<Plane>("yzq", camera, planeAttribute);

    ObjectAttribute fontAttribute; 
    fontAttribute.pos = {25.0f, 1150.0f, 0.0f};
    fontAttribute.scale.x = 1.0f;
    fontAttribute.color = {0.6f, 0.6f, 0.2f};
    auto font = make_shared<Font>("FPS", camera, fontAttribute);

    ObjectAttribute cubeAttribute;
    cubeAttribute.pos = {-3.0f, 0.8f, -1.0f};
    cubeAttribute.color = {0.8f, 0.5f, 0.2f};
    cubeAttribute.scale = {0.3f, 0.4f, 0.3f};
    std::string gamePath = "image/container.jpg";
    gamePath = RES_PATH + gamePath;
    cubeAttribute.textureID = loadTexture(gamePath.c_str());
    auto cube = make_shared<Cube>("cube", camera, cubeAttribute);

    window->addObject(font);
    window->addObject(plane);
    window->addObject(yzqPlane);
    window->addObject(cube);
    window->renderLoop();
    
    return 0;
}
