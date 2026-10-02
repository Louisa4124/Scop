#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <cmath>

class Matrix
{
public:
    // Genere une matrice de rotation 4x4 autour de l'axe Y
    static std::array<float, 16> createRotationY(float angleDegrees);
};

#endif