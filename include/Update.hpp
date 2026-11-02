#pragma once

#include <array>
#include <vector>
#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Buffer.hpp"
#include "../include/Matrix.hpp"
#include "../include/Input.hpp"

// Structure pour encapsuler l'etat et les objets
struct AppState
{
    Window &window;
    Shader &shaderProgram;
    VertexArray &vao;
    IndexBuffer &ebo;
    const std::array<float, 16> &projectionMatrix;
    unsigned int textureID;
    Input &inputManager;

    float   rotationAngleY;
    float   rotationAngleX;
    float   rotationAngleZ;
    float   modelPosX;
    float   modelPosY;
    float   modelPosZ;
    
    bool    targetShowTexture;
    float   transitionFactor;
    int     colorMode;
    float   lastTime;
};

std::array<float, 16> computeModelMatrix(float rotationAngleY, float rotationAngleX, float rotationAngleZ,
                                       float modelPosX, float modelPosY, float modelPosZ);

void update(AppState &app);