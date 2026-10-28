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

    bool plusKeyPressedLastFrame = false;
    bool minusKeyPressedLastFrame = false;
    int speedIndex = 0; // 0.5f, 0.8f, 1.0f, 1.5f, 2.0f

    void updateRotationSpeed();
    float getRotationSpeed() const;

public:
    float processInputRotationY(Window &window);
    float processInputRotationX(Window &window);
};

#endif