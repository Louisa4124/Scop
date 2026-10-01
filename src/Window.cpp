#include "../include/Window.hpp"

Window::Window(int width, int height, const char *title)
{
    // Initialisation de GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creation de la fenêtre GLFW
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(m_window);

    // Initialisation de GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(-1);
    }
}

Window::~Window()
{
    if (m_window)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}