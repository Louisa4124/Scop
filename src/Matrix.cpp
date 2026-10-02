#include "../include/Matrix.hpp"

std::array<float, 16> Matrix::createRotationY(float angleDegrees)
{
    float angleRad = angleDegrees * (static_cast<float>(M_PI) / 180.0f);
    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);

    return {
         cosA, 0.0f, -sinA, 0.0f,
         0.0f, 1.0f,  0.0f, 0.0f,
         sinA, 0.0f,  cosA, 0.0f,
         0.0f, 0.0f,  0.0f, 1.0f
    };
}