#include "BmpLoader.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

bool loadBMP(const char* filepath, std::vector<unsigned char>& pixels, int& width, int& height)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file)
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier BMP : " << filepath << std::endl;
        return false;
    }

    char header[54];
    file.read(header, 54);

    if (!file || header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Erreur : Le fichier n'a pas un format BMP valide." << std::endl;
        return false;
    }

    width = *reinterpret_cast<int*>(&header[18]);
    height = *reinterpret_cast<int*>(&header[22]);
    short bpp = *reinterpret_cast<short*>(&header[28]);

    if (bpp != 24)
    {
        std::cerr << "Erreur : Seuls les fichiers BMP 24-bits sont pris en charge (celui-ci est en " << bpp << " bits)." << std::endl;
        return false;
    }

    int rowSize = ((width * 3 + 3) / 4) * 4;
    std::vector<unsigned char> bmpData(rowSize * std::abs(height));

    file.read(reinterpret_cast<char*>(bmpData.data()), bmpData.size());
    file.close();

    pixels.resize(width * std::abs(height) * 3);

    for (int y = 0; y < std::abs(height); ++y)
    {
        int srcRow = (height > 0) ? (std::abs(height) - 1 - y) : y;
        for (int x = 0; x < width; ++x)
        {
            int srcIndex = srcRow * rowSize + x * 3;
            int destIndex = (y * width + x) * 3;

            pixels[destIndex + 0] = bmpData[srcIndex + 2];
            pixels[destIndex + 1] = bmpData[srcIndex + 1];
            pixels[destIndex + 2] = bmpData[srcIndex + 0];
        }
    }

    return true;
}