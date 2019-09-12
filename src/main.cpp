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
    LightManager::getInstance()->addLight(1, light);

    pos = {4.0, -0.2, -2.0};
    color = {1.0f, 0.0f, 0.0f};
    auto light2 = std::make_shared<Light>(pos, color);
    LightManager::getInstance()->addLight(2, light2);

    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT, camera);
    Controller::getInstance()->setCamera(camera);

    ObjectAttribute groundAttribute;
    std::string path = "image/car.jpg";
    path = RES_PATH + path;
    groundAttribute.textureID = loadTexture(path.c_str());
    groundAttribute.pos = {0.0f, 0.0f, 0.0f};
    groundAttribute.scale = {14.2f, 1.0f, 8.0f};
    auto plane = make_shared<Plane>("ground", camera, groundAttribute, true);
    plane->init();

    ObjectAttribute planeAttribute;
    std::string yzqPath = "image/yzq.jpg";
    yzqPath = RES_PATH + yzqPath;
    planeAttribute.textureID = loadTexture(yzqPath.c_str());
    planeAttribute.pos = {0.0f, 1.0f, 2.0f};
    glm::vec3 axis = {1.0f, 0.0f, 0.0f};
    planeAttribute.quat = glm::angleAxis(glm::radians(270.0f), axis) * planeAttribute.quat;
    auto yzqPlane = make_shared<Plane>("yzq", camera, planeAttribute, true);
    yzqPlane->init();

    ObjectAttribute windowAttribute;
    std::string windowPath = "image/window.png";
    windowPath = RES_PATH + windowPath;
    windowAttribute.textureID = loadTexture(windowPath.c_str());
    windowAttribute.pos = {-2.5f, -0.1f, -2.0f};
    windowAttribute.quat = glm::angleAxis(glm::radians(270.0f), axis) * windowAttribute.quat;
    windowAttribute.scale = {0.3f, 0.4f, 0.3f};
    auto windowPlane = make_shared<Plane>("window", camera, windowAttribute, true);
    windowPlane->init();

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
    auto cube = make_shared<Cube>("cube", camera, cubeAttribute, true);
    cube->init();

    ObjectAttribute sphereAttribute;
    sphereAttribute.pos = {3.0f, 0.2f, -1.0f};
    sphereAttribute.color = {1.0f, 1.0f, 1.0f};
    sphereAttribute.scale = {0.3f, 0.4f, 0.3f};
    std::string spherePath = "image/grass.jpeg";
    spherePath = RES_PATH + spherePath;
    sphereAttribute.textureID = loadTexture(spherePath.c_str());
    auto sphere = make_shared<Sphere>("sphere", camera, sphereAttribute, true);
    sphere->init();

    ObjectAttribute lightAttribute;
    lightAttribute.pos = light->getPos();
    lightAttribute.color = light->getColor();
    lightAttribute.scale = {0.15f, 0.2f, 0.15f};
    auto lightCube = make_shared<Sphere>("light1", camera, lightAttribute);
    lightCube->init();

    lightAttribute.pos = light2->getPos();
    lightAttribute.color = light2->getColor();
    lightAttribute.scale = {0.15f, 0.2f, 0.15f};
    auto lightCube2 = make_shared<Cube>("light2", camera, lightAttribute);
    lightCube2->init();

    window->addObject(plane);
    window->addObject(yzqPlane);
    window->addObject(cube);
    window->addObject(sphere);
    window->addObject(lightCube);
    window->addObject(lightCube2);
    window->addObject(windowPlane);
    window->addObject(font);
    window->renderLoop();
    
    return 0;
}
