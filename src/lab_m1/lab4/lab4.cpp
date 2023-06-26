#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    originX = 0;
    originY = 0;
    width = 0;
    height = 0;
    glm::ivec2 resolution = window->GetResolution();
    width = resolution.x / 8;
    height = resolution.y / 8;
    cubeRotate = 0;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, 0, -1.5f);
    modelMatrix *= transform3D::RotateOY(cubeRotate);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, 0);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    
    DrawCoordinateSystem();

    
    glViewport(originX, originY, width, height);
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, 0, -1.5f);
    modelMatrix *= transform3D::RotateOY(cubeRotate);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, 0);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    DrawCoordinateSystem();

}


void Lab4::FrameEnd()
{
    
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W)) {
        this->translateZ -= deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        this->translateZ += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_A)) {
        this->translateX -= deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_D)) {
        this->translateX += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_R)) {
        this->translateY += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_F)) {
        this->translateY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        this->scaleX += deltaTime;
        this->scaleY += deltaTime;
        this->scaleZ += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_2)) {
        this->scaleX -= deltaTime;
        this->scaleY -= deltaTime;
        this->scaleZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        this->angularStepOX += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_4)) {
        this->angularStepOX -= deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_5)) {
        this->angularStepOY += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_6)) {
        this->angularStepOY -= deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_7)) {
        this->angularStepOZ += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_8)) {
        this->angularStepOZ -= deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_9)) {
        this->cubeRotate += deltaTime;
    }
    else if (window->KeyHold(GLFW_KEY_0)) {
        this->cubeRotate -= deltaTime;
    }

}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    else if (key == GLFW_KEY_I) {
        this->originY += 10;
    }
    else if (key == GLFW_KEY_K) {
        this->originY -= 10;
    }
    else if (key == GLFW_KEY_J) {
        this->originX -= 10;
    }
    else if (key == GLFW_KEY_L) {
        this->originX += 10;
    }
    else if (key == GLFW_KEY_U) {
        this->width += 10;
        this->height += 10;
        this->originX -= 5;
        this->originY -= 5;
    }
    else if (key == GLFW_KEY_O) {
        this->width -= 10;
        this->height -= 10;
        this->originX += 5;
        this->originY += 5;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
