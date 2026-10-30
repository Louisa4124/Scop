#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Buffer.hpp"
#include "../include/color.hpp"
#include "../include/Matrix.hpp"
#include "../include/ObjLoader.hpp"
#include "../include/BmpLoader.hpp"
#include "../include/Input.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <GLFW/glfw3.h>

float WIDTH = 800.0f;
float HEIGHT = 800.0f;

int main(int argc, char **argv)
{
    // Initialisation de la fenêtre GLFW et GLEW
    Window window(WIDTH, HEIGHT, "Scop - OpenGL");

    // Chargement et compilation des shaders depuis leurs fichiers
    Shader shaderProgram("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!ObjLoader::loadOBJ("./assets/42.obj", vertices, indices))
        return -1;
    std::cout << "Nombre de sommets : " << vertices.size() / 5 << std::endl; // Mis à jour pour refléter les 5 floats (x,y,z,u,v)
    std::cout << "Nombre d'indices : " << indices.size() << std::endl;

    // Vertex Buffer Object (VBO) => Stocke les donnees
    //                            => permet d'envoyer autant de donnees que possible a la carte graphique
    // Vertex Array Object (VAO) => Decrit les donnees
    // Element Buffer Objects (EBO) => Tampon qui memorise des indices (pour decider quels sommets sont a afficher et dans quel ordre)

    // 0. Création et liaison du VAO (Vertex Array Object)
    VertexArray vao;
    vao.bind();

    // 1. Copier les sommets dans un tampon VBO pour qu’OpenGL les utilise
    // GL_STATIC_DRAW : les données ne seront pas modifiées (ou rarement)
    // GL_DYNAMIC_DRAW : les données seront souvent modifiées
    // GL_STREAM_DRAW : les données seront modifiées à chaque affichage.
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
    if (loadBMP("./assets/mlp.bmp", texturePixels, width, height))
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

        std::cout << "Texture BMP chargée avec succès ! (" << width << "x" << height << ")" << std::endl;
    }
    else
    {
        std::cout << "Pas de texture chargée, passage en mode couleur unie/gris." << std::endl;
    }

    float rotationAngleY = 0.0f;
    float rotationAngleX = 0.0f;
    Input inputManager;
    bool targetShowTexture = false; // on desactive par defaut la texture
    float transitionFactor = 0.0f; 
    float lastTime = glfwGetTime();

    // matrice de projection perspective (pouur ne pas avoir une image distordu a cause du ratio)
    std::array<float, 16> projectionMatrix = Matrix::createPerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);

    while (!window.shouldClose())
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (inputManager.processTextureToggle(window))
        {
            targetShowTexture = !targetShowTexture;
            std::cout << "Transition vers : " << (targetShowTexture ? "Texturé" : "Gris/Couleur") << std::endl;
        }

        // transition progressive du mode couleur a texture, ou l'inverse
        float transitionSpeed = 2.0f;
        if (targetShowTexture)
        {
            transitionFactor += transitionSpeed * deltaTime;
            if (transitionFactor > 1.0f) transitionFactor = 1.0f;
        }
        else
        {
            transitionFactor -= transitionSpeed * deltaTime;
            if (transitionFactor < 0.0f) transitionFactor = 0.0f;
        }

        rotationAngleY += inputManager.processInputRotationY(window);
        rotationAngleX += inputManager.processInputRotationX(window);

        std::array<float, 16> rotationMatrixY = Matrix::createRotationY(-90.0f + rotationAngleY);
        std::array<float, 16> rotationMatrixX = Matrix::createRotationX(rotationAngleX);

        // Matrice de translation pour dezoomer l'objet 
        std::array<float, 16> translationMatrix = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, -3.0f, 1.0f
        };

        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        // Récupération de l'emplacement de u_Model et envoi de la matrice
        GLint modelLoc = glGetUniformLocation(shaderProgram.getID(), "u_Model");

        // N'envoie pas deux matrices séparées au même uniform.
        // Il faut les combiner puis envoyer le résultat une seule fois.
        std::array<float, 16> rotMatrix = Matrix::multiply(rotationMatrixY, rotationMatrixX);
        std::array<float, 16> modelMatrix = Matrix::multiply(translationMatrix, rotMatrix);

        std::array<float, 16> mvpMatrix = Matrix::multiply(projectionMatrix, modelMatrix);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, mvpMatrix.data());

        glUniform1f(glGetUniformLocation(shaderProgram.getID(), "u_Transition"), transitionFactor);

        // Ajout des textures si il y en a
        if (textureID != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(glGetUniformLocation(shaderProgram.getID(), "texture1"), 0);
        }

        // Mise à jour de la couleur
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        float redValue = sin(timeValue) * 2.0f + 0.5f;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram.getID(), "ourColor");
        glUniform4f(vertexColorLocation, 0.8f, greenValue, 1.0f, 0.8f);

        // GL_LINE : dessine les lignes entre les vertices sans remplir l'interieur
        // GL_FILL : l'interieur est plein
        // GL_POINT : dessines uniquement les vertices
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Afficher un objet
        vao.bind();
        glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0);
        vao.unbind();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}