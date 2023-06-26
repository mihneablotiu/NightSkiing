#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

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

        void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
                                     Texture2D* texture, implemented::Camera* myCamera,
                                     glm::mat4 &projectionMatrix);
        void Tema3::addARenderedObject(std::string object);

        void Tema3::generateTree(float xPosition, float zPosition);
        void Tema3::generateStone(float xPosition, float zPosition);
        void Tema3::generateGift(float xPosition, float zPosition);
        void Tema3::generatePole(float xPosition, float zPosition);
        bool Tema3::collisionDectection(glm::vec2 playerPosition, glm::vec2 objectPosition,
                                        glm::vec2 playerDeplasament, glm::vec2 objectDeplasament);

    protected:
        int isSlope;
        int fov;
        float deltaTime;
        float myMouseX, myMouseY;
        float rotationAngle;
        int chanceOfSpawningAnObject;
        bool gameFinished;
        int score;

        std::vector<std::string> possibleObjects;
        std::vector<std::pair<glm::vec2, std::string>> renderedObjects;

        glm::ivec2 resolution;
        implemented::Camera* myCamera;

        // The text rendered on the screen
        gfxc::TextRenderer* currentText;

        glm::vec3 initialSkierPosition;
        glm::vec3 cameraOffset;
        glm::mat4 projectionMatrix;
        std::unordered_map<std::string, Texture2D*> mapTextures;
    };
}
