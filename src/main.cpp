#include <iostream>
#include <memory>
#include <vector>

#include <Config.h>

#include "window/Window.h"
#include "controller/Controller.h"

#include "object/Plane.h"
#include "object/Font.h"
#include "object/Cube.h"
#include "object/Sphere.h"
#include "object/Model.h"

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
    groundAttribute.pos = {0.0f, -1.0f, 0.0f};
    groundAttribute.scale = {244.0f, 1.0f, 160.0f};
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
    windowAttribute.pos = {-2.0f, -0.0f, -2.0f};
    windowAttribute.quat = glm::angleAxis(glm::radians(270.0f), axis) * windowAttribute.quat;
    windowAttribute.scale = {0.3f, 0.3f, 0.3f};
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
    cubeAttribute.scale = {0.3f, 0.3f, 0.3f};
    std::string gamePath = "image/container.jpg";
    gamePath = RES_PATH + gamePath;
    cubeAttribute.textureID = loadTexture(gamePath.c_str());
    auto cube = make_shared<Cube>("cube", camera, cubeAttribute, true);
    cube->init();

    cubeAttribute.pos = {0.0f, 2.0f, 5.0f};
    cubeAttribute.scale = {1.5f, 1.5f, 1.5f};

    std::vector<std::string> faces;
    std::string envPath = "image/skybox/";
    envPath = RES_PATH + envPath;
    std::string bottom = envPath + "bottom.jpg";
    std::string back = envPath + "back.jpg";
    std::string front = envPath + "front.jpg";
    std::string left = envPath + "left.jpg";
    std::string right = envPath + "right.jpg";
    std::string top = envPath + "top.jpg";

    faces.push_back(right);
    faces.push_back(left);
    faces.push_back(top);
    faces.push_back(bottom);
    faces.push_back(front);
    faces.push_back(back);

    cubeAttribute.envTextureID = loadCubemap(faces);
    auto refCube = make_shared<Cube>("refCube", camera, cubeAttribute, false, true);
    refCube->init();

    ObjectAttribute sphereAttribute;
    sphereAttribute.pos = {3.0f, 0.2f, -1.0f};
    sphereAttribute.color = {1.0f, 1.0f, 1.0f};
    sphereAttribute.scale = {0.3f, 0.3f, 0.3f};
    std::string spherePath = "image/grass.jpeg";
    spherePath = RES_PATH + spherePath;
    sphereAttribute.textureID = loadTexture(spherePath.c_str());
    auto sphere = make_shared<Sphere>("sphere", camera, sphereAttribute, true);
    sphere->init();

    ObjectAttribute lightAttribute;
    lightAttribute.pos = light->getPos();
    lightAttribute.color = light->getColor();
    lightAttribute.scale = {0.15f, 0.15f, 0.15f};
    auto lightCube = make_shared<Sphere>("light1", camera, lightAttribute);
    lightCube->init();

    lightAttribute.pos = light2->getPos();
    lightAttribute.color = light2->getColor();
    lightAttribute.scale = {0.15f, 0.15f, 0.15f};
    auto lightCube2 = make_shared<Cube>("light2", camera, lightAttribute);
    lightCube2->init();

    ObjectAttribute modelAttribute;
    modelAttribute.pos = {0.5f, -0.5f, -6.0f};
    modelAttribute.quat *= glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelAttribute.quat *= glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelAttribute.scale = {0.2f, 0.2f, 0.2f};
    std::string modelPath = "model/Bambo_House.blend";
    modelPath = RES_PATH + modelPath;
    auto model = std::make_shared<Model>("model", camera, modelAttribute, modelPath, true);
    model->init();

    modelAttribute.pos = {-2.0f, 2.5f, -5.0f};
    modelAttribute.scale = {0.1f, 0.1f, 0.1f};
    modelAttribute.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelAttribute.quat *= glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    std::string uh60Path = "model/UH60/uh60.obj";
    uh60Path = RES_PATH + uh60Path;
    auto uh60 = std::make_shared<Model>("model", camera, modelAttribute, uh60Path, true);
    uh60->init();

    modelAttribute.pos = {1.5f, -0.5f, -1.0f};
    modelAttribute.scale = {0.05f, 0.05f, 0.05f};
    modelAttribute.quat = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    std::string treePath = "model/trees/trees9.3ds";
    treePath = RES_PATH + treePath;
    auto tree = std::make_shared<Model>("model", camera, modelAttribute, treePath, true);
    tree->init();

    ObjectAttribute cityAttribute;
    cityAttribute.pos = {-10.0f, 0.0f, -50.0f};
    cityAttribute.scale = {0.3f, 0.3f, 0.3f};
    std::string cityPath = "model/Organodron City/Organodron City.obj";
    cityPath = RES_PATH + cityPath;
    auto city = std::make_shared<Model>("model", camera, cityAttribute, cityPath, true);
    city->init();

    window->addObject(plane);
    window->addObject(cube);
    window->addObject(refCube);
    window->addObject(sphere);
    window->addObject(windowPlane);
    window->addObject(model);
    window->addObject(uh60);
    window->addObject(tree);
    window->addObject(city);
    window->addObject(lightCube);
    window->addObject(lightCube2);
    window->addObject(font);
    window->renderLoop();
    
    return 0;
}
