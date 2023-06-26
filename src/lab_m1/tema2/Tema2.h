#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

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
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera* camera, glm::mat4 &projectionMatrix);
        void determineObstaclesTrack();
        void determineTranslationPosition(float deltaTimeSeconds);
        void generateTrees(std::vector<VertexFormat> treePositions, glm::mat4 projectionMatrix, implemented::Camera* camera);
        void lapDoneFunction(glm::vec3& carPosition, float deltaTimeSeconds);
    protected:
        // The track vertices and the positions of the trees
        std::vector<VertexFormat> initialVertices, vertices, fewerVertices, firstTreePositions, secondTreePositions;
        
        // The positions of the dinamically moved cars
        std::vector<std::vector<VertexFormat>> tracks;

        // The positions of where the enemies cars are translated
        std::vector<glm::vec3> translationPositions;

        // The angle of rotation of the player's car
        std::vector<float> rotationPositions;

        // The current vertex of which each enemy car is positioned at a certain moment
        std::vector<int> currentVertex;
        
        // The color of the track
        glm::vec3 color;

        // The current position of the player's car
        glm::vec3 carPosition;

        // The offset of the camera from the player's car
        glm::vec3 cameraOffset;

        // The Model matrix and projection matrix used for the two cameras
        glm::mat4 modelMatrix;
        glm::mat4 projectionMatrix, projectionMatrixMinimap;

        // The cameras
        implemented::Camera* camera;
        implemented::Camera* minimapCamera;

        // The text rendered on the screen
        gfxc::TextRenderer* currentText;

        // The resolution of the screen
        glm::ivec2 resolution;

        GLenum polygonMode;
        int numberOfTrees, numberOfCars;
        float rotatePlayerCar, lengthOfTheCar;
        float speed, deltaTimeMinimap;
        float timeSinceLastLap;
        bool collisionDetected, lapDone, halfLapDone;
        int fov;
        int numberOfLaps;
    };
}
