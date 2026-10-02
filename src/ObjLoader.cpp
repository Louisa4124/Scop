#include "../include/ObjLoader.hpp"

bool ObjLoader::loadOBJ(const std::string &filePath, 
                        std::vector<float> &outVertices, 
                        std::vector<unsigned int> &outIndices,
                        bool normalize,
                        float targetScale)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier : " << filePath << std::endl;
        return false;
    }

    std::vector<float> rawPositions;
    std::string line;

    // On initialise la BB (Bounnding Box)
    // => rectangle imaginaire qui contient entirement un object/scene
    float minX = std::numeric_limits<float>::max(), maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max(), maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max(), maxZ = std::numeric_limits<float>::lowest();

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        // Parsing des sommets v (cas simple uniquement)
        if (prefix == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;

            rawPositions.push_back(x);
            rawPositions.push_back(y);
            rawPositions.push_back(z);

            minX = std::min(minX, x); 
            maxX = std::max(maxX, x);
            minY = std::min(minY, y); 
            maxY = std::max(maxY, y);
            minZ = std::min(minZ, z); 
            maxZ = std::max(maxZ, z);
        }
        // Parsing des faces f
        else if (prefix == "f")
        {
            std::string vertexStr;
            std::vector<unsigned int> faceIndices;

            while (ss >> vertexStr)
            {
                std::stringstream vss(vertexStr);
                std::string indexStr;
                std::getline(vss, indexStr);

                if (!indexStr.empty())
                {
                    unsigned int vIdx = std::stoi(indexStr) - 1;
                    faceIndices.push_back(vIdx);
                }
            }

            // triangle fan
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
            {
                outIndices.push_back(faceIndices[0]);
                outIndices.push_back(faceIndices[i]);
                outIndices.push_back(faceIndices[i + 1]);
            }
        }
    }

    file.close();

    if (rawPositions.empty())
    {
        std::cerr << "Erreur : Aucun sommets trouver dans " << filePath << std::endl;
        return false;
    }

    // On centre et met le modele a l'echelle
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    float sizeX = maxX - minX;
    float sizeY = maxY - minY;
    float sizeZ = maxZ - minZ;
    float maxExtent = std::max({sizeX, sizeY, sizeZ});

    float scaleFactor = (normalize && maxExtent > 0.0f) ? (targetScale / maxExtent) : 1.0f;

    outVertices.reserve(rawPositions.size());
    for (size_t i = 0; i < rawPositions.size(); i += 3)
    {
        outVertices.push_back((rawPositions[i] - centerX) * scaleFactor);
        outVertices.push_back((rawPositions[i + 1] - centerY) * scaleFactor);
        outVertices.push_back((rawPositions[i + 2] - centerZ) * scaleFactor);
    }

    return true;
}