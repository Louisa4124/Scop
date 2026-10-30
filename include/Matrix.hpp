#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

class Matrix
{
public:
    // Genere une matrice de rotation 4x4 autour de l'axe Y
    static std::array<float, 16> createRotationY(float angleDeg);
    // Genere une matrice de rotation 4x4 autour de l'axe X
    static std::array<float, 16> createRotationX(float angleDeg);
    
    static std::array<float, 16> multiply(const std::array<float, 16> &a,
        const std::array<float, 16> &b);
        
    static std::array<float, 16> createPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane);
};

#endif