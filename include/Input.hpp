#ifndef INPUT_HPP
#define INPUT_HPP

class Window;

class Input
{
private:
    // Rotations automatiques
    bool    oneKeyPressedLastFrame = false;
    bool    twoKeyPressedLastFrame = false;
    bool    threeKeyPressedLastFrame = false;
    bool    rotationToggledY = false;
    bool    rotationToggledX = false;
    bool    rotationToggledZ = false;

    // Gestion de la vitesse de rotation
    bool    plusKeyPressedLastFrame = false;
    bool    minusKeyPressedLastFrame = false;
    int     speedIndex = 4; // 0.5f, 0.8f, 1.0f, 1.5f, 2.0f, 4.0f, 15.0f

    // Activer/desac la texture
    bool    tKeyPressedLastFrame = false;
    bool    textureModeEnabled = false;

    void    updateRotationSpeed();
    float   getRotationSpeed() const;

public:
    float   processInputRotationY(Window &window);
    float   processInputRotationX(Window &window);
    float   processInputRotationZ(Window &window);

    float   processInputTranslationX(Window &window);
    float   processInputTranslationY(Window &window);
    float   processInputTranslationZ(Window &window);

    bool    processTextureToggle(Window &window);
    bool    processColorModeToggle(Window &window);
};

#endif