#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Buffer.hpp"
#include "../include/color.hpp"
#include "../include/Matrix.hpp"
#include "../include/ObjLoader.hpp"
#include "../include/BmpLoader.hpp"
#include "../include/Input.hpp"
#include "../include/Update.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <GLFW/glfw3.h>

const float WIDTH = 800.0f;
const float HEIGHT = 800.0f;

void printHelp(const char *progName)
{
    std::cout << "\n******** Manuel d'utilisation de Scop *********\n\n";
    std::cout << "Utilisation : \n" << progName << " <fichier.obj> [texture.bmp]\n";
    std::cout << "PS: la texture est optionnel \n\n";
    std::cout << "Commandes disponibles :\n";
    std::cout << "- w, a, s, d, q, e : Déplacer le modèle sur les axes x, y et z\n";
    std::cout << "- ↑, ↓, →, ←, 0, . : Tourner le modèle manuellement\n";
    std::cout << "- 1, 2, 3 : Activer les rotations constantes du modèle\n";
    std::cout << "- c : Changer le mode de couleur\n";
    std::cout << "- t : Activer / désactiver la texture\n";
    std::cout << "- m : Changer le mode d'affichage (fil de fer, remplissage, points)\n";
    std::cout << "- +, - : Augmenter / réduire la vitesse de rotation\n";
    std::cout << "- Esc : Fermer la fenêtre\n";
}

bool hasEnding(const std::string &fullString, const std::string &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    return false;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "\nErreur : Aucun fichier `.obj` fourni.\n\n";
        std::cerr << "Utilisation : \n" << argv[0] << " <fichier.obj> [texture.bmp]\n";
        std::cerr << "PS: la texture est optionnel \n\n";
        std::cerr << "Tapez '" << argv[0] << " -h' ou '" << argv[0] << " --help' pour afficher l'aide et les commandes.\n";
        return 1;
    }

    std::string arg1 = argv[1];
    if (arg1 == "-h" || arg1 == "--help")
    {
        printHelp(argv[0]);
        return 0;
    }

    if (!hasEnding(arg1, ".obj") && !hasEnding(arg1, ".OBJ"))
    {
        std::cerr << "\nErreur : Seul les fichiers .obj sont acceptés.\n\n";
        std::cerr << "Utilisation : \n" << argv[0] << " <fichier.obj> [texture.bmp]\n";
        return 1;
    }

    std::string objPath = argv[1];
    std::string bmpPath = "";

    if (argc >= 3)
    {
        std::string arg2 = argv[2];
        if (!hasEnding(arg2, ".bmp") && !hasEnding(arg2, ".BMP"))
        {
            std::cerr << "\nErreur : Seul les fichiers .bmp sont acceptés pour les textures.\n";
            return 1;
        }
        bmpPath = arg2;
    }

    // Initialisation de la fenêtre GLFW et GLEW
    Window window(WIDTH, HEIGHT, "Scop");

    // Chargement et compilation des shaders depuis leurs fichiers
    Shader shaderProgram("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!ObjLoader::loadOBJ(objPath.c_str(), vertices, indices))
    {
        std::cerr << "Erreur : Impossible de charger le fichier OBJ : " << objPath << std::endl;
        return -1;
    }

    // Vertex Buffer Object (VBO) => Stocke les donnees
    //                            => permet d'envoyer autant de donnees que possible a la carte graphique
    // Vertex Array Object (VAO) => Decrit les donnees
    // Element Buffer Objects (EBO) => Tampon qui memorise des indices (pour decider quels sommets sont a afficher et dans quel ordre)

    // 0. Création et liaison du VAO (Vertex Array Object)
    VertexArray vao;
    vao.bind();

    // 1. Copier les sommets dans un tampon VBO pour qu’OpenGL les utilise
    VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(float));

    // 2. Copier les indices dans un tampon EBO
    IndexBuffer ebo(indices.data(), indices.size());

    // 3. Initialiser les pointeurs d’attributs de sommets dans le VAO
    size_t stride = 5 * sizeof(float);
    vao.addBuffer(vbo, 0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0); // 0 : Position (vec3) -> commence à l'indice 0
    vao.addBuffer(vbo, 1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float))); // 1 : Coordonnees de texture (vec2) -> commence apres les 3 floats de position

    vao.unbind();

    glEnable(GL_DEPTH_TEST);

    int width, height;
    std::vector<unsigned char> texturePixels;

    unsigned int textureID = 0;
    if (!bmpPath.empty() && loadBMP(bmpPath.c_str(), texturePixels, width, height))
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Paramètres de texture OpenGL
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Envoi des pixels bruts à OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePixels.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Pas de texture chargée, passage en mode couleur." << std::endl;

    float rotationAngleY = 0.0f;
    float rotationAngleX = 0.0f;
    float rotationAngleZ = 0.0f;
    Input inputManager;
    bool targetShowTexture = false; // on desactive par defaut la texture
    float transitionFactor = 0.0f;

    int colorMode = 0; // mode de couleur par défaut => gris
    float lastTime = glfwGetTime();

    float modelPosX = 0.0f;
    float modelPosY = 0.0f;
    float modelPosZ = -2.0f;

    // matrice de projection perspective (pouur ne pas avoir une image distordu a cause du ratio)
    std::array<float, 16> projectionMatrix = Matrix::createPerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);

    // Initialisation de la structure d'état pour la fonction update
    AppState appState = {
        window, shaderProgram, vao, ebo, projectionMatrix, textureID,
        inputManager, rotationAngleY, rotationAngleX, rotationAngleZ,
        modelPosX, modelPosY, modelPosZ,
        targetShowTexture, transitionFactor, colorMode, lastTime
    };

    while (!window.shouldClose())
        update(appState);

    return 0;
}