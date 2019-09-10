#include <iostream>
#include <memory>

#include <Config.h>

#include "window/Window.h"
#include "controller/Controller.h"

#include "object/Plane.h"
#include "object/Font.h"

#include "Common.h"

using namespace std;

int main(int, char**) {

    auto camera =  make_shared<Camera>();
    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT, camera);
    Controller::getInstance()->setCamera(camera);

    ObjectAttribute groundAttribute; 
    std::string path = "image/wood.png";
    path = RES_PATH + path;
    groundAttribute.textureID = loadTexture(path.c_str());
    groundAttribute.pos = {0.0f, 0.0f, 1.0f};
    glm::vec3 axis = {1.0f, 0.0f, 0.0f};
    groundAttribute.quat = glm::angleAxis(glm::radians(90.0f), axis) * groundAttribute.quat;
    groundAttribute.scale = {5.0f, 5.0f, 5.0f};
    auto plane = make_shared<Plane>("ground", camera, groundAttribute);

    ObjectAttribute planeAttribute;
    std::string yzqPath = "image/yzq.jpg";
    yzqPath = RES_PATH + yzqPath;
    planeAttribute.textureID = loadTexture(yzqPath.c_str());
    planeAttribute.pos = {0.0f, 1.0f, 3.0f};
    auto yzqPlane = make_shared<Plane>("yzq", camera, planeAttribute);

    ObjectAttribute fontAttribute; 
    fontAttribute.pos = {25.0f, 25.0f, 0.0f};
    fontAttribute.scale.x = 1.0f;
    fontAttribute.color = {0.0f, 0.0f, 0.8f};
    auto font = make_shared<Font>("FPS", camera, fontAttribute);

    window->addObject(font);
    window->addObject(plane);
    window->addObject(yzqPlane);
    window->renderLoop();
    
    return 0;
}
