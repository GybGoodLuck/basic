#include <iostream>
#include <memory>
#include "window/Window.h"

#include "object/Plane.h"
#include "object/Font.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int, char**) {

    auto window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT);
    auto plane = make_shared<Plane>();
    auto font = make_shared<Font>();

    window->addObject(plane);
    window->addObject(font);
    window->renderLoop();
    
    return 0;
}
