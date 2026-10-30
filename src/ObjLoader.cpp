#include "../include/ObjLoader.hpp"
#include <map>

// Structure pour relier les sommets avec leur textures
struct VertexKey {
    int v;
    int vt;
    bool operator<(const VertexKey& o) const {
        if (v != o.v) 
            return v < o.v;
        return vt < o.vt;
    }
};

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
    std::vector<float> rawTexCoords;

    struct FaceVertex {
        int vIdx;
        int vtIdx;
    };
    std::vector<std::vector<FaceVertex>> faces;

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
        // Parsing des textures vt
        else if (prefix == "vt")
        {
            float u, v;
            ss >> u >> v;
            rawTexCoords.push_back(u);
            rawTexCoords.push_back(v);
        }
        // Parsing des faces f
        else if (prefix == "f")
        {
            std::string vertexStr;
            std::vector<FaceVertex> faceVertices;

            while (ss >> vertexStr)
            {
                std::stringstream vss(vertexStr);
                std::string vStr, vtStr, vnStr;
                std::getline(vss, vStr, '/'); // indice position
                std::getline(vss, vtStr, '/'); // indice texture
                std::getline(vss, vnStr, '/'); // indice normale

                int vIdx = vStr.empty() ? -1 : std::stoi(vStr) - 1;
                int vtIdx = vtStr.empty() ? -1 : std::stoi(vtStr) - 1;

                faceVertices.push_back({vIdx, vtIdx});
            }

            // triangle fan
            for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
            {
                std::vector<FaceVertex> triangle = {faceVertices[0], faceVertices[i], faceVertices[i + 1]};
                faces.push_back(triangle);
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

    std::map<VertexKey, unsigned int> uniqueVertices;
    outVertices.clear();
    outIndices.clear();

    for (const auto &triangle : faces)
    {
        for (const auto &fv : triangle)
        {
            VertexKey key = {fv.vIdx, fv.vtIdx};

            if (uniqueVertices.find(key) == uniqueVertices.end())
            {
                unsigned int newIndex = static_cast<unsigned int>(outVertices.size() / 5);
                uniqueVertices[key] = newIndex;

                float x = rawPositions[fv.vIdx * 3 + 0];
                float y = rawPositions[fv.vIdx * 3 + 1];
                float z = rawPositions[fv.vIdx * 3 + 2];

                outVertices.push_back((x - centerX) * scaleFactor);
                outVertices.push_back((y - centerY) * scaleFactor);
                outVertices.push_back((z - centerZ) * scaleFactor);

                if (fv.vtIdx >= 0 && (size_t)(fv.vtIdx * 2 + 1) < rawTexCoords.size())
                {
                    outVertices.push_back(rawTexCoords[fv.vtIdx * 2 + 0]);
                    outVertices.push_back(rawTexCoords[fv.vtIdx * 2 + 1]);
                }
                else
                {
                    outVertices.push_back(0.0f);
                    outVertices.push_back(0.0f);
                }
            }

            outIndices.push_back(uniqueVertices[key]);
        }
    }

    return true;
}