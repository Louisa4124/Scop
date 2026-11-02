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

int main(int argc, char **argv)
{
    // Initialisation de la fenêtre GLFW et GLEW
    Window window(WIDTH, HEIGHT, "Scop - OpenGL");

    // Chargement et compilation des shaders depuis leurs fichiers
    Shader shaderProgram("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!ObjLoader::loadOBJ("./assets/fox.obj", vertices, indices))
        return -1;

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
    if (loadBMP("./assets/fox.bmp", texturePixels, width, height))
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