#include "../include/Update.hpp"
#include "../include/color.hpp"
#include <GLFW/glfw3.h>

std::array<float, 16> computeModelMatrix(float rotationAngleY, float rotationAngleX, float rotationAngleZ,
                                       float modelPosX, float modelPosY, float modelPosZ)
{
    // on cree les matrices de rotations
    std::array<float, 16> rotationMatrixY = Matrix::createRotationY(-90.0f + rotationAngleY);
    std::array<float, 16> rotationMatrixX = Matrix::createRotationX(rotationAngleX);
    std::array<float, 16> rotationMatrixZ = Matrix::createRotationZ(rotationAngleZ);

    // on cree les matrices de translations
    std::array<float, 16> translationMatrixX = Matrix::createTranslationX(modelPosX);
    std::array<float, 16> translationMatrixY = Matrix::createTranslationY(modelPosY);
    std::array<float, 16> translationMatrixZ = Matrix::createTranslationZ(modelPosZ);

    // on les combine
    std::array<float, 16> transXY = Matrix::multiply(translationMatrixX, translationMatrixY);
    std::array<float, 16> transMatrix = Matrix::multiply(transXY, translationMatrixZ);

    // on combine les rotations
    std::array<float, 16> rotXY = Matrix::multiply(rotationMatrixY, rotationMatrixX);
    std::array<float, 16> rotMatrix = Matrix::multiply(rotXY, rotationMatrixZ);

    // matrice finale: rotation + translation
    std::array<float, 16> modelMatrix = Matrix::multiply(transMatrix, rotMatrix);

    return modelMatrix;
}

void update(AppState &app)
{
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - app.lastTime;
    app.lastTime = currentTime;

    // activation de la texture
    if (app.inputManager.processTextureToggle(app.window))
        app.targetShowTexture = !app.targetShowTexture;

    if (app.inputManager.processColorModeToggle(app.window))
        app.colorMode = (app.colorMode + 1) % 4; // alterne entre 4 mode de couleur

    // transition progressive du mode couleur a texture, ou l'inverse
    float transitionSpeed = 2.0f;
    if (app.targetShowTexture)
    {
        app.transitionFactor += transitionSpeed * deltaTime;
        if (app.transitionFactor > 1.0f)
            app.transitionFactor = 1.0f;
    }
    else
    {
        app.transitionFactor -= transitionSpeed * deltaTime;
        if (app.transitionFactor < 0.0f)
            app.transitionFactor = 0.0f;
    }

    app.rotationAngleY += app.inputManager.processInputRotationY(app.window);
    app.rotationAngleX += app.inputManager.processInputRotationX(app.window);
    app.rotationAngleZ += app.inputManager.processInputRotationZ(app.window);
    
    app.modelPosX += app.inputManager.processInputTranslationX(app.window) * deltaTime;
    app.modelPosY += app.inputManager.processInputTranslationY(app.window) * deltaTime;
    app.modelPosZ += app.inputManager.processInputTranslationZ(app.window) * deltaTime;

    utils::glClearColorHex(0x2b2c41);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.shaderProgram.use();

    // Récupération de l'emplacement de u_Model et envoi de la matrice
    GLint modelLoc = glGetUniformLocation(app.shaderProgram.getID(), "u_Model");

    // Calcul de la matrice modèle via la fonction dédiée
    std::array<float, 16> modelMatrix = computeModelMatrix(app.rotationAngleY, app.rotationAngleX, app.rotationAngleZ,
                                                           app.modelPosX, app.modelPosY, app.modelPosZ);

    std::array<float, 16> mvpMatrix = Matrix::multiply(app.projectionMatrix, modelMatrix);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, mvpMatrix.data());

    glUniform1f(glGetUniformLocation(app.shaderProgram.getID(), "u_Transition"), app.transitionFactor);
    glUniform1i(glGetUniformLocation(app.shaderProgram.getID(), "u_ColorMode"), app.colorMode);

    // Ajout des textures si il y en a
    if (app.textureID != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, app.textureID);
        glUniform1i(glGetUniformLocation(app.shaderProgram.getID(), "texture1"), 0);
    }

    // GL_LINE : dessine les lignes entre les vertices sans remplir l'interieur
    // GL_FILL : l'interieur est plein
    // GL_POINT : dessines uniquement les vertices
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Afficher un objet
    app.vao.bind();
    glDrawElements(GL_TRIANGLES, app.ebo.getCount(), GL_UNSIGNED_INT, 0);
    app.vao.unbind();

    app.window.swapBuffers();
    app.window.pollEvents();
}