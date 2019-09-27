#include "Window.h"
#include "../controller/Controller.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    MousePosition position;
    position.x = xpos;
    position.y = ypos;
    Controller::getInstance()->setMousePosition(position);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Controller::getInstance()->processMouse(button, action, mods);
}

Window::Window(int width, int height, const Camera::Ptr& camera) 
        : m_width(width), m_height(height), m_camera(camera) {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    m_GLWindow = glfwCreateWindow(m_width, m_height, "Basic", NULL, NULL);
    if (m_GLWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_GLWindow);
    glfwSetFramebufferSizeCallback(m_GLWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_GLWindow, mouse_callback);
    glfwSetMouseButtonCallback(m_GLWindow, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Window::addObject(const std::shared_ptr<Object>& object) {
    m_renderList.push_back(object);
}

void Window::removeObject(const std::shared_ptr<Object>& object) {

    for (auto it = m_renderList.begin(); it != m_renderList.end(); ) {
        if (*it == object) {
            m_renderList.erase(it++);
        }
    }
}

int count = 0;
auto start = std::chrono::system_clock::now();
void Window::renderLoop() {

    while(!glfwWindowShouldClose(m_GLWindow)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        for (auto object : m_renderList)
        {
            update(object);
            object->render();
        }

        glfwSwapBuffers(m_GLWindow);
        glfwPollEvents();

        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        count++;

        if (int(duration.count()) >= 1000) {
            m_fps = count;
            start = std::chrono::system_clock::now();
            count = 0;
        }
    }

    glfwTerminate();
}

void Window::update(const std::shared_ptr<Object>& object) {

    switch (object->type())
    {
        case FONT:
            {
                std::string fps = "FPS ";
                fps = fps + std::to_string(m_fps);
                auto font = std::static_pointer_cast<Font>(object);
                
                if (font->getName() == "FPS") {
                    font->setText(fps);
                }
            }
            break;
        case PLANE:
            {
                break;
            }
        case CUBE:
            {
                if (object->getName() == "cube") {
                    glm::vec3 axis = {1.0f, 0.0f, 1.0f};
                    auto rotate = glm::angleAxis(glm::radians(1.0f), axis);
                    object->setQua(rotate * object->getQua());
                }
                break;
            }
        default:
            break;
    }

    object->update();
}

void Window::processInput()
{
    if(glfwGetKey(m_GLWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_GLWindow, true);

    if (glfwGetKey(m_GLWindow, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_GLWindow, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_GLWindow, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_GLWindow, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processKeyboard(RIGHT, deltaTime);
}