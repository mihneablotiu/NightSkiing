#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        /* The model matrix for each part of the duck
        and the moving matrix (the same for each component) */
        glm::mat3 modelMatrix, movingMatrix;

        // The vector with the resolution of the screen
        glm::ivec2 resolution;

        /* The angle of the wings rotationand the angle
        of the random start of the duck */
        float angularStepWings, randomStart;

        /* Booleans for checking if the maximum rotation of the
        wings was done, if a duck is on the screen or if a duck
        was hit with a bullet */
        bool isMaximumWings, isADuckSpawned, duckWasHit;

        /* The x and y positions of the beak of the duck for colisions*/
        float outsideScreenVertical, outsideScreenHorizontal;

        /* The translation length for the X and Y axis and the initial
        speed of the duck */
        float translateX, translateY, initialSpeed;

        /* The length and width of the duck and the time spent since
        the appearance of the last duck */
        float lengthOfTheDuck, widthOfTheDuck, timeSpentForADuck;

        /* The time to rotate the multiplier on the screen */
        float multiplierTime, multiplierAngle;
        int multiplier;
        bool finishAnimation;
        std::string multiplierString;

        /* The number of the ducks that spawned until a certain moment*/
        int numberOfDucks;

        /* The positions of the mouse cursor */
        int myMouseX, myMouseY;

        /* The three lifes and the three bullets */
        bool firstMiss, secondMiss, thirdMiss;
        bool firstBullet, secondBullet, thirdBullet;

        /* How many bullets were shot and the current score*/
        int bulletsShot, score;

        /* The random colors vectors for the wings, head and beak */
        glm::vec3 randomColorWings, randomColorHead, randomColorBeak;

        /* A string for the score that is going to be written on the screen*/
        std::string textScore;

        /* The text rederer according to the ocw tutorial */
        gfxc::TextRenderer* currentText;
    };
}
