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

    bool onePressed = (glfwGetKey(w, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_KP_1) == GLFW_PRESS);
    if (onePressed && !oneKeyPressedLastFrame)
        rotationToggledY = !rotationToggledY;
    oneKeyPressedLastFrame = onePressed;

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

    bool twoPressed = (glfwGetKey(w, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_KP_2) == GLFW_PRESS);
    if (twoPressed && !twoKeyPressedLastFrame)
        rotationToggledX = !rotationToggledX;
    twoKeyPressedLastFrame = twoPressed;

    bool upPressed = (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS);
    bool downPressed = (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (downPressed)
        return -getRotationSpeed();

    if (upPressed || rotationToggledX)
        return getRotationSpeed();

    return 0.0f;
}

float Input::processInputRotationZ(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    updateRotationSpeed();

    bool threePressed = (glfwGetKey(w, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_KP_3) == GLFW_PRESS);
    if (threePressed && !threeKeyPressedLastFrame)
        rotationToggledZ = !rotationToggledZ;
    threeKeyPressedLastFrame = threePressed;

    bool zeroPressed = (glfwGetKey(w, GLFW_KEY_KP_0) == GLFW_PRESS);
    bool pointPressed = (glfwGetKey(w, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS);

    if (zeroPressed)
        return -getRotationSpeed();

    if (pointPressed || rotationToggledZ)
        return getRotationSpeed();

    return 0.0f;
}

float Input::processInputTranslationX(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    float speed = getRotationSpeed() * 0.5f;

    bool dPressed = (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS);
    bool aPressed = (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS);

    if (dPressed) 
        return speed;  // Droite (+X)
    if (aPressed)
        return -speed; // Gauche (-X)

    return 0.0f;
}

float Input::processInputTranslationY(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    float speed = getRotationSpeed() * 0.5f;

    bool ePressed = (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS);
    bool qPressed = (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS);

    if (ePressed) 
        return speed;  // Monter (+Y)
    if (qPressed) 
        return -speed; // Descendre (-Y)

    return 0.0f;
}

float Input::processInputTranslationZ(Window &window)
{
    GLFWwindow *w = glfwGetCurrentContext();
    if (!w)
        return 0.0f;

    float speed = getRotationSpeed() * 0.5f;

    bool wPressed = (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS);
    bool sPressed = (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS);

    if (wPressed) 
        return speed;  // Avancer (+Z)
    if (sPressed) 
        return -speed; // Reculer (-Z)

    return 0.0f;
}

bool Input::processTextureToggle(Window &window)
{
    GLFWwindow *w = window.getNativeWindow();
    if (!w)
        return false;

    bool tPressed = (glfwGetKey(w, GLFW_KEY_T) == GLFW_PRESS);
    bool toggled = false;

    if (tPressed && !tKeyPressedLastFrame)
    {
        textureModeEnabled = !textureModeEnabled;
        toggled = true;
    }

    tKeyPressedLastFrame = tPressed;
    return toggled;
}

bool Input::processColorModeToggle(Window &window) {
    static bool cKeyWasPressed = false;
    bool cKeyIsPressed = (glfwGetKey(window.getNativeWindow(), GLFW_KEY_C) == GLFW_PRESS);
    if (cKeyIsPressed && !cKeyWasPressed) {
        cKeyWasPressed = true;
        return true;
    }
    if (!cKeyIsPressed) {
        cKeyWasPressed = false;
    }
    return false;
}