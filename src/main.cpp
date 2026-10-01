#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Buffer.hpp"
#include "../include/color.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    // Initialisation de la fenêtre GLFW et GLEW
    Window window(800, 600, "Scop - OpenGL");

    // Chargement et compilation des shaders depuis leurs fichiers
    Shader shaderProgram("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // Notons que l’on commence à 0!
        0, 1, 3, // premier triangle
        1, 2, 3  // second triangle
    };

    // Vertex Buffer Object (VBO) => Stocke les donnees
    //                            => permet d'envoyer autant de donnees que possible a la carte graphique
    // Vertex Array Object (VAO) => Decrit les donnees
    // Element Buffer Objects (EBO) => Tampon qui memorise des indices (pour decider quels sommets sont a afficher et dans quel ordre)
    
    // 0. Création et liaison du VAO (Vertex Array Object)
    VertexArray vao;
    vao.bind();

    // 1. Copier les sommets dans un tampon VBO pour qu’OpenGL les utilise
    // GL_STATIC_DRAW : les données ne seront pas modifiées (ou rarement) ;
    // GL_DYNAMIC_DRAW : les données seront souvent modifiées ;
    // GL_STREAM_DRAW : les données seront modifiées à chaque affichage.
    VertexBuffer vbo(vertices, sizeof(vertices));

    // 2. Copier les indices dans un tampon EBO
    IndexBuffer ebo(indices, 6);

    // 3. Initialiser les pointeurs d’attributs de sommets dans le VAO
    vao.addBuffer(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Déliage pour éviter les modifications accidentelles
    vao.unbind();

    // 4. Boucle de rendu
    while (!window.shouldClose())
    {
        utils::glClearColorHex(0x000000);
        glClear(GL_COLOR_BUFFER_BIT);

        // Utiliser notre program shader pour l'affichage d'un objet
        shaderProgram.use();

        // GL_LINE : dessine les lignes entre les vertices sans remplir l'interieur
        // GL_FILL : l'interieur est plein
        // GL_POINT : dessines uniquement les vertices
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Afficher un objet
        vao.bind();
        glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0);
        vao.unbind();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}