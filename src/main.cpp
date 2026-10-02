#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Buffer.hpp"
#include "../include/color.hpp"
#include "../include/Matrix.hpp"
#include "../include/ObjLoader.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>

int main(int argc, char **argv)
{
    // Initialisation de la fenêtre GLFW et GLEW
    Window window(800, 600, "Scop - OpenGL");

    // Chargement et compilation des shaders depuis leurs fichiers
    Shader shaderProgram("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!ObjLoader::loadOBJ("./assets/42.obj", vertices, indices))
    {
        return -1;
    }
    std::cout << "Nombre de sommets : " << vertices.size() / 3 << std::endl;
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
    vao.addBuffer(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    vao.unbind();

    glEnable(GL_DEPTH_TEST);

    float i = 0.0f;

    // 4. Boucle de rendu
    while (!window.shouldClose())
    {
        i += 0.5f;

        std::array<float, 16> rotationMatrix = Matrix::createRotationY(-90.0f + i);
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        // Récupération de l'emplacement de u_Model et envoi de la matrice
        GLint modelLoc = glGetUniformLocation(shaderProgram.getID(), "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotationMatrix.data());

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