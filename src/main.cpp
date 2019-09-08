#include <iostream>
#include <memory>
#include "window/Window.h"

#include "object/Plane.h"
#include "object/Font.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int, char**) {

    auto camera =  make_shared<Camera>();
    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT);
    auto plane = make_shared<Plane>("YZQ", camera);
    auto font = make_shared<Font>("FPS", camera);

    window->addObject(font);
    window->addObject(plane);
    window->renderLoop();
    
    return 0;
}
