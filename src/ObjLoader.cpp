#include "../include/ObjLoader.hpp"
#include <map>
#include <algorithm>
#include <cmath>

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

    bool hasRawUVs = !rawTexCoords.empty();

    outVertices.clear();
    outIndices.clear();

    if (hasRawUVs)
    {
        // cas ou le fichier a ses propres coord de textures u/v
        std::map<VertexKey, unsigned int> uniqueVertices;

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
    }
    else
    {
        // si le fichier obj n'a pas de u/v
        unsigned int currentIndex = 0;

        for (const auto &triangle : faces)
        {
            if (triangle.size() < 3) continue;

            int v0Idx = triangle[0].vIdx;
            int v1Idx = triangle[1].vIdx;
            int v2Idx = triangle[2].vIdx;

            float x0 = rawPositions[v0Idx * 3 + 0], y0 = rawPositions[v0Idx * 3 + 1], z0 = rawPositions[v0Idx * 3 + 2];
            float x1 = rawPositions[v1Idx * 3 + 0], y1 = rawPositions[v1Idx * 3 + 1], z1 = rawPositions[v1Idx * 3 + 2];
            float x2 = rawPositions[v2Idx * 3 + 0], y2 = rawPositions[v2Idx * 3 + 1], z2 = rawPositions[v2Idx * 3 + 2];

            float ax = x1 - x0, ay = y1 - y0, az = z1 - z0;
            float bx = x2 - x0, by = y2 - y0, bz = z2 - z0;

            float nx = ay * bz - az * by;
            float ny = az * bx - ax * bz;
            float nz = ax * by - ay * bx;

            float len = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (len > 0.0f) {
                nx /= len; 
                ny /= len; 
                nz /= len;
            }

            float absNx = std::abs(nx);
            float absNy = std::abs(ny);
            float absNz = std::abs(nz);

            for (const auto &fv : triangle)
            {
                float x = rawPositions[fv.vIdx * 3 + 0];
                float y = rawPositions[fv.vIdx * 3 + 1];
                float z = rawPositions[fv.vIdx * 3 + 2];

                outVertices.push_back((x - centerX) * scaleFactor);
                outVertices.push_back((y - centerY) * scaleFactor);
                outVertices.push_back((z - centerZ) * scaleFactor);

                float u = 0.0f, v = 0.0f;

                // Projection selon l'axe dominant de la normale de la face
                if (absNz >= absNx && absNz >= absNy) {
                    // Face avant / arrière (Plan XY)
                    u = (sizeX > 0.0f) ? (x - minX) / sizeX : 0.0f;
                    v = (sizeY > 0.0f) ? (y - minY) / sizeY : 0.0f;
                } else if (absNx >= absNy && absNx >= absNz) {
                    // Faces gauche / droite (Plan YZ)
                    u = (sizeZ > 0.0f) ? (z - minZ) / sizeZ : 0.0f;
                    v = (sizeY > 0.0f) ? (y - minY) / sizeY : 0.0f;
                } else {
                    // Faces dessus / dessous (Plan XZ)
                    u = (sizeX > 0.0f) ? (x - minX) / sizeX : 0.0f;
                    v = (sizeZ > 0.0f) ? (z - minZ) / sizeZ : 0.0f;
                }

                outVertices.push_back(u);
                outVertices.push_back(v);

                outIndices.push_back(currentIndex++);
            }
        }
    }

    return true;
}