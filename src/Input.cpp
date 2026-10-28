#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/Input.hpp"
#include "../include/Window.hpp"

float Input::processInputRotationY(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(w, GLFW_TRUE);
        return 0.0f;
    }

    bool aPressed = (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS);
    if (aPressed && !aKeyPressedLastFrame)
        rotationToggledY = !rotationToggledY;
    aKeyPressedLastFrame = aPressed;

    bool leftPressed = (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS);
    bool rightPressed = (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS);

    if (rightPressed)
        return -0.5f;

    if (leftPressed || rotationToggledY)
        return 0.5f;

    return 0.0f;
}

float Input::processInputRotationX(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    bool sPressed = (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS);
    if (sPressed && !sKeyPressedLastFrame)
        rotationToggledX = !rotationToggledX;
    sKeyPressedLastFrame = sPressed;

    bool upPressed = (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS);
    bool downPressed = (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (downPressed)
        return -0.5f;

    if (upPressed || rotationToggledX)
        return 0.5f;

    return 0.0f;
}
