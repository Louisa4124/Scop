#include "../include/Matrix.hpp"
#include <cmath>

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

std::array<float, 16> Matrix::createRotationX(float angleDeg)
{
    const float pi = 3.14159265358979323846f;
    const float rad = angleDeg * (pi / 180.0f);
    const float c = std::cos(rad);
    const float s = std::sin(rad);

    // Colonne-major
    return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,    c,    s, 0.0f,
        0.0f,   -s,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

std::array<float, 16> Matrix::multiply(const std::array<float, 16> &a,
                                       const std::array<float, 16> &b)
{
    std::array<float, 16> r = {0.0f};

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            r[col * 4 + row] =
                a[0 * 4 + row] * b[col * 4 + 0] +
                a[1 * 4 + row] * b[col * 4 + 1] +
                a[2 * 4 + row] * b[col * 4 + 2] +
                a[3 * 4 + row] * b[col * 4 + 3];
        }
    }

    return r;
}