#include <iostream>
#include <memory>

#include <Config.h>

#include "window/Window.h"
#include "controller/Controller.h"

#include "object/Plane.h"
#include "object/Font.h"
#include "object/Cube.h"
#include "object/Sphere.h"

#include "Common.h"

using namespace std;

int main(int, char**) {

    auto camera =  make_shared<Camera>();
    glm::vec3 pos = {-3.0, 0.0, -2.0};
    glm::vec3 color = {0.0f, 0.0f, 1.0f};
    auto light = std::make_shared<Light>(pos, color);
    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT, camera);
    Controller::getInstance()->setCamera(camera);

    ObjectAttribute groundAttribute; 
    std::string path = "image/car.jpg";
    path = RES_PATH + path;
    groundAttribute.textureID = loadTexture(path.c_str());
    groundAttribute.pos = {0.0f, 0.0f, 0.0f};
    groundAttribute.scale = {14.2f, 1.0f, 8.0f};
    auto plane = make_shared<Plane>("ground", camera, light, groundAttribute);
    plane->init();

    ObjectAttribute planeAttribute;
    std::string yzqPath = "image/yzq.jpg";
    yzqPath = RES_PATH + yzqPath;
    planeAttribute.textureID = loadTexture(yzqPath.c_str());
    planeAttribute.pos = {0.0f, 1.0f, 2.0f};
    glm::vec3 axis = {1.0f, 0.0f, 0.0f};
    planeAttribute.quat = glm::angleAxis(glm::radians(270.0f), axis) * planeAttribute.quat;
    auto yzqPlane = make_shared<Plane>("yzq", camera, light, planeAttribute);
    yzqPlane->init();

    ObjectAttribute fontAttribute; 
    fontAttribute.pos = {25.0f, 1150.0f, 0.0f};
    fontAttribute.scale.x = 1.0f;
    fontAttribute.color = {0.6f, 0.6f, 0.2f};
    auto font = make_shared<Font>("FPS", camera, fontAttribute);
    font->init();

    ObjectAttribute cubeAttribute;
    cubeAttribute.pos = {-3.0f, 0.2f, -1.0f};
    cubeAttribute.color = {0.2f, 0.5f, 0.2f};
    cubeAttribute.scale = {0.3f, 0.4f, 0.3f};
    std::string gamePath = "image/container.jpg";
    gamePath = RES_PATH + gamePath;
    cubeAttribute.textureID = loadTexture(gamePath.c_str());
    auto cube = make_shared<Cube>("cube", camera, light, cubeAttribute);
    cube->init();

    ObjectAttribute sphereAttribute;
    sphereAttribute.pos = {3.0f, 0.2f, -1.0f};
    sphereAttribute.color = {1.0f, 1.0f, 1.0f};
    sphereAttribute.scale = {0.3f, 0.4f, 0.3f};
    std::string spherePath = "image/grass.jpeg";
    spherePath = RES_PATH + spherePath;
    sphereAttribute.textureID = loadTexture(spherePath.c_str());
    auto sphere = make_shared<Sphere>("sphere", camera, light, sphereAttribute);
    sphere->init();

    ObjectAttribute lightAttribute;
    lightAttribute.pos = light->getPos();
    lightAttribute.color = light->getColor();
    lightAttribute.scale = {0.15f, 0.2f, 0.15f};
    auto lightCube = make_shared<Cube>("light", camera, lightAttribute);
    lightCube->init();

    window->addObject(plane);
    window->addObject(yzqPlane);
    window->addObject(cube);
    window->addObject(sphere);
    window->addObject(lightCube);
    window->addObject(font);
    window->renderLoop();
    
    return 0;
}
