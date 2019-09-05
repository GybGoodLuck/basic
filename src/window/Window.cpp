#include "Window.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height) : m_width(width), m_height(height) {
    glfwInit();
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
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

void Window::renderLoop() {

    while(!glfwWindowShouldClose(m_GLWindow)) {
        processInput(m_GLWindow);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        for (auto object : m_renderList)
        {
            object->render();
        }
        
        glfwSwapBuffers(m_GLWindow);
        glfwPollEvents();
    }

    glfwTerminate();
}