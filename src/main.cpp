#include <iostream>
#include <memory>

#include <Config.h>

#include "window/Window.h"

#include "object/Plane.h"
#include "object/Font.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int, char**) {

    auto camera =  make_shared<Camera>();
    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT);

    ObjectAttribute planeAttribute; 
    std::string path = "image/yzq.jpg";
    path = RES_PATH + path;
    planeAttribute.textureID = loadTexture(path.c_str());
    auto plane = make_shared<Plane>("YZQ", camera, planeAttribute);
    ObjectAttribute fontAttribute; 
    fontAttribute.pos = {25.0f, 25.0f, 0.0f};
    fontAttribute.scale.x = 1.0f;
    fontAttribute.color = {0.0f, 0.0f, 0.8f};
    auto font = make_shared<Font>("FPS", camera, fontAttribute);

    window->addObject(font);
    window->addObject(plane);
    window->renderLoop();
    
    return 0;
}
