#include <iostream>
#include <memory>
#include "window/Window.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main(int, char**) {

    shared_ptr<Window> window = make_shared<Window>(SCR_WIDTH, SCR_HEIGHT);
    auto plane = make_shared<Plane>();
    window->addObject(plane);
    window->renderLoop();
    
    return 0;
}
