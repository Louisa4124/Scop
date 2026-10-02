#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

class ObjLoader
{
public:
    // Charge les sommets 'v' ET les faces 'f' d'un fichier .obj
    static bool loadOBJ(const std::string &filePath, 
                        std::vector<float> &outVertices, 
                        std::vector<unsigned int> &outIndices,
                        bool normalize = true,
                        float targetScale = 1.0f);
};

#endif 