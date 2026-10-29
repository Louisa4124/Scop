#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/Input.hpp"
#include "../include/Window.hpp"

void Input::updateRotationSpeed()
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return;

    // gerer '+' avec clavier + pave numerique
    bool plusPressed =
        (glfwGetKey(w, GLFW_KEY_EQUAL) == GLFW_PRESS) ||
        (glfwGetKey(w, GLFW_KEY_KP_ADD) == GLFW_PRESS);

    // gerer '-' avec clavier + pave numerique
    bool minusPressed =
        (glfwGetKey(w, GLFW_KEY_MINUS) == GLFW_PRESS) ||
        (glfwGetKey(w, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);

    if (plusPressed && !plusKeyPressedLastFrame && speedIndex < 6)
        ++speedIndex;

    if (minusPressed && !minusKeyPressedLastFrame && speedIndex > 0)
        --speedIndex;

    plusKeyPressedLastFrame = plusPressed;
    minusKeyPressedLastFrame = minusPressed;
}

float Input::getRotationSpeed() const
{
    static const float speeds[7] = {0.5f, 0.8f, 1.0f, 1.5f, 2.0f, 4.0f, 15.0f};
    return speeds[speedIndex];
}

float Input::processInputRotationY(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    updateRotationSpeed();

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
        return -getRotationSpeed();

    if (leftPressed || rotationToggledY)
        return getRotationSpeed();

    return 0.0f;
}

float Input::processInputRotationX(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    updateRotationSpeed();

    bool sPressed = (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS);
    if (sPressed && !sKeyPressedLastFrame)
        rotationToggledX = !rotationToggledX;
    sKeyPressedLastFrame = sPressed;

    bool upPressed = (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS);
    bool downPressed = (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (downPressed)
        return -getRotationSpeed();

    if (upPressed || rotationToggledX)
        return getRotationSpeed();

    return 0.0f;
}
