#ifndef INPUT_HPP
#define INPUT_HPP

class Window;

class Input
{
private:
    bool aKeyPressedLastFrame = false;
    bool sKeyPressedLastFrame = false;
    bool rotationToggledY = false;
    bool rotationToggledX = false;

public:
    float processInputRotationY(Window &window);
    float processInputRotationX(Window &window);
};

#endif