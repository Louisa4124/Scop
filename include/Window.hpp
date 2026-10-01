#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    GLFWwindow *getNativeWindow() const { return m_window; }

private:
    GLFWwindow *m_window;
};

#endif // WINDOW_HPP