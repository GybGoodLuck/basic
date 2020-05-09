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

    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT, camera);
    Controller::getInstance()->setCamera(camera);

    ObjectAttribute fontAttribute; 
    fontAttribute.pos = {25.0f, 1150.0f, 0.0f};
    fontAttribute.scale.x = 1.0f;
    fontAttribute.color = {0.6f, 0.6f, 0.2f};
    auto font = make_shared<Font>("FPS", camera, fontAttribute);
    font->init();

    ObjectAttribute modelAttribute;
    modelAttribute.pos = {500.0f, -200.0f, -290.0f};

    modelAttribute.scale = {0.3f, 0.3f, 0.3f};
    std::string modelPath = "model/jiqiren.FBX";
    modelPath = RES_PATH + modelPath;
    auto model = std::make_shared<Model>("model", camera, modelAttribute, modelPath, true);
    model->init();

    window->addObject(model);
    window->addObject(font);
    window->renderLoop();
    
    return 0;
}
