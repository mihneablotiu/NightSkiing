#include "lab_m1/tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

#include "lab_m1/lab4/transform3D.h"
#include "lab_m1/tema2/track.h"
#include "lab_m1/tema2/tree.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/tema2/collision.h"
#include "components/text_renderer.h"
#include "lab_m1/tema1/createRectangle.h"

#define MAX_LAPS 3

using namespace std;
using namespace m1;


Tema3::Tema3()
{
    this->initialSkierPosition = glm::vec3(0, 0, 0);
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

    this->gameFinished = false;
    this->score = 0;
    this->fov = 60;
    this->rotationAngle = 0;
    this->projectionMatrix = glm::perspective(glm::radians((float)this->fov), window->props.aspectRatio, 0.01f, 200.0f);
    this->cameraOffset = glm::vec3(0.0f, 8.0f, 8.0f);

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        this->mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "skier_texture.jpg").c_str(), GL_REPEAT);
        this->mapTextures["skier_texture"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "skies.jpg").c_str(), GL_REPEAT);
        this->mapTextures["skies"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "tree.jpg").c_str(), GL_REPEAT);
        this->mapTextures["tree"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves.jpg").c_str(), GL_REPEAT);
        this->mapTextures["leaves"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.jpg").c_str(), GL_REPEAT);
        this->mapTextures["stone"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        this->mapTextures["gift"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pole.jpg").c_str(), GL_REPEAT);
        this->mapTextures["pole"] = texture;
    }

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("slope");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("stone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    this->myCamera = new implemented::Camera();
    this->myCamera->Set(this->initialSkierPosition + this->cameraOffset, this->initialSkierPosition, glm::vec3(0, 1, 0));
    this->myCamera->distanceToTarget = distance(this->initialSkierPosition, this->myCamera->position);

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("Tema3Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    this->possibleObjects.push_back("tree");
    this->possibleObjects.push_back("gift");
    this->possibleObjects.push_back("pole");
    this->possibleObjects.push_back("stone");

    /* The initialization of the text rendered on the screen */
    this->resolution = window->GetResolution();
    this->currentText = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    this->currentText->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
}


void Tema3::FrameStart()
{
    if (this->gameFinished == false) {
        // Clears the color buffer (using the previously set color) and depth buffer
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        resolution = window->GetResolution();
        // Sets the screen area where to draw
        glViewport(0, 0, resolution.x, resolution.y);
    }
    else {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, resolution.x, resolution.y);

        char textScore[100] = "Game Over";
        currentText->RenderText(textScore, (float)resolution.x / 3.5f, (float)resolution.y / 4, 2.0f);

        char scoreValue[100];
        sprintf(scoreValue, "Final score: %d", this->score);
        currentText->RenderText(scoreValue, (float)resolution.x / 3.5f, (float)resolution.y / 3, 1.5f);

        char restartMessage[100] = "Press R to restart";
        currentText->RenderText(restartMessage, (float)resolution.x / 3.5f, (float)resolution.y / 2.5, 1.0f);
    }
}


void Tema3::Update(float deltaTimeSeconds)
{
    if (this->gameFinished == false) {
        this->deltaTime = deltaTimeSeconds;

        this->myCamera->Set(this->initialSkierPosition + this->cameraOffset, this->initialSkierPosition, glm::vec3(0, 1, 0));
        this->myCamera->distanceToTarget = distance(this->initialSkierPosition, this->myCamera->position);

        this->isSlope = 1;
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(this->initialSkierPosition.x, this->initialSkierPosition.y, this->initialSkierPosition.z);
            modelMatrix *= transform3D::RotateOX(glm::radians(-80.0));
            modelMatrix *= transform3D::Scale(50.0f, 50.0f, 1.0f);
            RenderSimpleMesh(meshes["slope"], shaders["Tema3Shader"], modelMatrix, mapTextures["snow"], myCamera, projectionMatrix);
        }

        this->isSlope = 0;

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(this->initialSkierPosition.x, this->initialSkierPosition.y, this->initialSkierPosition.z);
            modelMatrix *= transform3D::RotateOX(glm::radians(10.0f));
            modelMatrix *= transform3D::RotateOY(this->rotationAngle);
            modelMatrix *= transform3D::Translate(0.0f, 0.57f, 0.0f);
            modelMatrix *= transform3D::Scale(0.55f, 1.0f, 0.45f);
            RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["skier_texture"], myCamera, projectionMatrix);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(this->initialSkierPosition.x, this->initialSkierPosition.y, this->initialSkierPosition.z);
            modelMatrix *= transform3D::RotateOX(glm::radians(10.0f));
            modelMatrix *= transform3D::RotateOY(this->rotationAngle);
            modelMatrix *= transform3D::Translate(0.0f, 1.35f, 0.0f);
            modelMatrix *= transform3D::Scale(0.65f, 0.65f, 0.65f);
            RenderSimpleMesh(meshes["stone"], shaders["Tema3Shader"], modelMatrix, mapTextures["skies"], myCamera, projectionMatrix);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(this->initialSkierPosition.x, this->initialSkierPosition.y, this->initialSkierPosition.z);
            modelMatrix *= transform3D::RotateOX(glm::radians(10.0f));
            modelMatrix *= transform3D::RotateOY(this->rotationAngle);
            modelMatrix *= transform3D::Translate(-0.2f, 0.0f, 0.0f);
            modelMatrix *= transform3D::Scale(0.15f, 0.2f, 2.0f);
            RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["skies"], myCamera, projectionMatrix);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(this->initialSkierPosition.x, this->initialSkierPosition.y, this->initialSkierPosition.z);
            modelMatrix *= transform3D::RotateOX(glm::radians(10.0f));
            modelMatrix *= transform3D::RotateOY(this->rotationAngle);
            modelMatrix *= transform3D::Translate(0.2f, 0.0f, 0.0f);
            modelMatrix *= transform3D::Scale(0.15f, 0.2f, 2.0f);
            RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["skies"], myCamera, projectionMatrix);
        }

        this->initialSkierPosition += glm::vec3(glm::sin(this->rotationAngle) * deltaTimeSeconds * 5,
            -deltaTimeSeconds * glm::tan(glm::radians(10.0f)) * 5,
            glm::cos(this->rotationAngle) * deltaTimeSeconds * 5);

        float newRotationAngle = glm::atan(this->myMouseX / this->myMouseY);
        if (glm::degrees(newRotationAngle) <= 55.0f && glm::degrees(newRotationAngle) >= -55.0f) {
            this->rotationAngle = glm::atan(this->myMouseX / this->myMouseY);
        }

        char scoreValue[100];
        sprintf(scoreValue, "Current score: %d", this->score);
        currentText->RenderText(scoreValue, (float)resolution.x / 20, (float)resolution.y / 12, 1.0f, glm::vec3(0, 0, 0));
    }
}


void Tema3::FrameEnd()
{
    if (this->gameFinished == false) {
        for (int i = 0; i < this->renderedObjects.size(); i++) {
            if (this->initialSkierPosition.z - this->renderedObjects.at(i).first.y >= 40.0f) {
                this->renderedObjects.erase(this->renderedObjects.begin() + i);
            }
        }

        for (int i = 0; i < this->renderedObjects.size(); i++) {
            float randomX = this->renderedObjects.at(i).first.x;
            float randomZ = this->renderedObjects.at(i).first.y;
            if (this->renderedObjects.at(i).second == "tree") {
                generateTree(randomX, randomZ);
            }
            else if (this->renderedObjects.at(i).second == "stone") {
                generateStone(randomX, randomZ);
            }
            else if (this->renderedObjects.at(i).second == "gift") {
                generateGift(randomX, randomZ);
            }
            else {
                generatePole(randomX, randomZ);
            }
        }

        srand(GetTickCount64());
        this->chanceOfSpawningAnObject = rand() % 100;
        if (this->chanceOfSpawningAnObject <= 5) {
            int randomIndex = rand() % 4;

            if (randomIndex == 0) {
                addARenderedObject("tree");
            }
            else if (randomIndex == 1) {
                addARenderedObject("gift");
            }
            else if (randomIndex == 2) {
                addARenderedObject("pole");
            }
            else {
                addARenderedObject("stone");
            }
        }

        for (int i = 0; i < this->renderedObjects.size(); i++) {
            glm::vec2 playerPosition = glm::vec2(this->initialSkierPosition.x, this->initialSkierPosition.z);
            glm::vec2 playerDeplasament = glm::vec2(0.275f, 0.225f);
            glm::vec2 objectPosition = this->renderedObjects.at(i).first;
            glm::vec2 objectDeplasament;

            if (this->renderedObjects.at(i).second == "tree") {
                objectDeplasament = glm::vec2(0.275, 0.225f);
                if (collisionDectection(playerPosition, objectPosition, playerDeplasament, objectDeplasament) == true) {
                    this->gameFinished = true;
                }
            }
            else if (this->renderedObjects.at(i).second == "stone") {
                objectDeplasament = glm::vec2(0.625f, 0.375f);
                if (collisionDectection(playerPosition, objectPosition, playerDeplasament, objectDeplasament) == true) {
                    this->gameFinished = true;
                }
            }
            else if (this->renderedObjects.at(i).second == "gift") {
                objectDeplasament = glm::vec2(0.275f, 0.275f);
                if (collisionDectection(playerPosition, objectPosition, playerDeplasament, objectDeplasament) == true) {
                    this->renderedObjects.erase(this->renderedObjects.begin() + i);
                    this->score++;
                }
            }
            else {
                objectDeplasament = glm::vec2(0.125f, 0.125f);
                if (collisionDectection(playerPosition, objectPosition, playerDeplasament, objectDeplasament) == true) {
                    this->gameFinished = true;
                }
            }
        }
    }
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_R) {
        if (this->gameFinished == true) {
            this->gameFinished = false;
            this->score = 0;
            this->initialSkierPosition = glm::vec3(0, 0, 0);
            this->renderedObjects.clear();
        }
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
} 


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // We make the variable change operation because the normal coordinate system
    // starts in left bottom of the screen but the coordinates of the mouse start
    // on top left of the screen
    this->myMouseY = mouseY - resolution.y / 2;
    this->myMouseX = mouseX - resolution.x / 2;
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture,
                             implemented::Camera* myCamera, glm::mat4 &projectionMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = myCamera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture != NULL) {
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
    }
    

    int timeLocation = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeLocation, (float)Engine::GetElapsedTime());

    int isSlopeLocation = glGetUniformLocation(shader->program, "isSlope");
    glUniform1i(isSlopeLocation, this->isSlope);

    int textureDeplasamentLocation = glGetUniformLocation(shader->program, "textureDeplasament");
    glUniform2f(textureDeplasamentLocation, this->initialSkierPosition.x / 100.0f, -this->initialSkierPosition.z / 100.0f);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::addARenderedObject(std::string object) {
    float minimumValueX = this->initialSkierPosition.x - 15.0f;
    float maximumValueX = this->initialSkierPosition.x + 15.0f;
    float randomX = rand() % (int)(maximumValueX - minimumValueX + 1) + minimumValueX;

    float minimumValueZ = this->initialSkierPosition.z + 10.0f;
    float maximumValueZ = this->initialSkierPosition.z + 20.0f;
    float randomZ = rand() % (int)(maximumValueZ - minimumValueZ + 1) + minimumValueZ;

    for (int i = 0; i < this->renderedObjects.size(); i++) {
        float currentObjectX = this->renderedObjects.at(i).first.x;
        float currentObjectZ = this->renderedObjects.at(i).first.y;

        glm::vec2 currentObjectPos = glm::vec2(currentObjectX, currentObjectZ);
        glm::vec2 currentObjectDepl;

        if (this->renderedObjects.at(i).second == "tree") {
            currentObjectDepl = glm::vec2(0.275, 0.225f);
        }
        else if (this->renderedObjects.at(i).second == "stone") {
            currentObjectDepl = glm::vec2(0.625f, 0.375f);
        }
        else if (this->renderedObjects.at(i).second == "gift") {
            currentObjectDepl = glm::vec2(0.275f, 0.275f);
        }
        else {
            currentObjectDepl = glm::vec2(0.125f, 0.125f);
        }

        glm::vec2 newObjectPos = glm::vec2(randomX, randomZ);
        glm::vec2 newObjectDepl;

        if (object == "tree") {
            newObjectDepl = glm::vec2(0.275, 0.225f);
        }
        else if (object == "stone") {
            newObjectDepl = glm::vec2(0.625f, 0.375f);
        }
        else if (object == "gift") {
            newObjectDepl = glm::vec2(0.275f, 0.275f);
        }
        else {
            newObjectDepl = glm::vec2(0.125f, 0.125f);
        }

        if (collisionDectection(currentObjectPos, newObjectPos, currentObjectDepl, newObjectDepl) == true) {
            return;
        }

    }

    this->renderedObjects.push_back(std::make_pair(glm::vec2(randomX, randomZ), object));
}

void Tema3::generateTree(float xPosition, float zPosition) {
    float yOffset = initialSkierPosition.y - (glm::tan(glm::radians(10.0f)) * (zPosition - initialSkierPosition.z));

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 0.6f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.55f, 1.2f, 0.45f);
    RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["tree"], myCamera, projectionMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 2.0f + yOffset, zPosition);
    RenderSimpleMesh(meshes["tree"], shaders["Tema3Shader"], modelMatrix, mapTextures["leaves"], myCamera, projectionMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 2.8f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.75f, 0.75f, 0.75f);
    RenderSimpleMesh(meshes["tree"], shaders["Tema3Shader"], modelMatrix, mapTextures["leaves"], myCamera, projectionMatrix);
}

void Tema3::generateStone(float xPosition, float zPosition) {
    float yOffset = initialSkierPosition.y - (glm::tan(glm::radians(10.0f)) * (zPosition - initialSkierPosition.z));

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 0.375f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.75f, 0.75f, 0.75f);
    RenderSimpleMesh(meshes["stone"], shaders["Tema3Shader"], modelMatrix, mapTextures["stone"], myCamera, projectionMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition + 0.5f, 0.275f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.55f);
    RenderSimpleMesh(meshes["stone"], shaders["Tema3Shader"], modelMatrix, mapTextures["stone"], myCamera, projectionMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition - 0.3f, 0.175f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.35f, 0.35f, 0.35f);
    RenderSimpleMesh(meshes["stone"], shaders["Tema3Shader"], modelMatrix, mapTextures["stone"], myCamera, projectionMatrix);
}

void Tema3::generateGift(float xPosition, float zPosition) {
    float yOffset = initialSkierPosition.y - (glm::tan(glm::radians(10.0f)) * (zPosition - initialSkierPosition.z));

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 0.275f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.55f);
    RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["gift"], myCamera, projectionMatrix);
}

void Tema3::generatePole(float xPosition, float zPosition) {
    float yOffset = initialSkierPosition.y - (glm::tan(glm::radians(10.0f)) * (zPosition - initialSkierPosition.z));

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 1.5f + yOffset, zPosition);
    modelMatrix *= transform3D::Scale(0.25f, 3.0f, 0.25f);
    RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["pole"], myCamera, projectionMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xPosition, 3.0f + yOffset, zPosition);
    modelMatrix *= transform3D::RotateOZ(glm::radians(90.0f));
    modelMatrix *= transform3D::Scale(0.35f, 3.0f, 0.35f);
    RenderSimpleMesh(meshes["body"], shaders["Tema3Shader"], modelMatrix, mapTextures["pole"], myCamera, projectionMatrix);
}

bool Tema3::collisionDectection(glm::vec2 playerPosition, glm::vec2 objectPosition,
                                glm::vec2 playerDeplasament, glm::vec2 objectDeplasament) {
    float playerMinimumX = playerPosition.x - playerDeplasament.x;
    float playerMaximumX = playerPosition.x + playerDeplasament.x;

    float playerMinimumZ = playerPosition.y - playerDeplasament.y;
    float playerMaximumZ = playerPosition.y + playerDeplasament.y;

    float objectMinimumX = objectPosition.x - objectDeplasament.x;
    float objectMaximumX = objectPosition.x + objectDeplasament.x;

    float objectMinimumZ = objectPosition.y - objectDeplasament.y;
    float objectMaximumZ = objectPosition.y + objectDeplasament.y;

    return (
        playerMinimumX <= objectMaximumX &&
        playerMaximumX >= objectMinimumX &&
        playerMinimumZ <= objectMaximumZ &&
        playerMaximumZ >= objectMinimumZ
        );
}
