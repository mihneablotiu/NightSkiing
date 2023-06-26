#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

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


Tema2::Tema2()
{
    this->speed = 4.0f;
    this->fov = 60;

    this->numberOfTrees = 0;
    this->numberOfCars = 4;

    this->numberOfLaps = -1;
    this->timeSinceLastLap = 0;

    this->lapDone = true;
    this->halfLapDone = true;
    this->polygonMode = GL_FILL;

    for (int i = 0; i < this->numberOfCars; i++) {
        this->translationPositions.push_back(glm::vec3(0, 0, 0));
        this->rotationPositions.push_back(0);
        this->currentVertex.push_back(0);
    }

    /* The initialization for the projection matrix of the normal view and minimap */
    projectionMatrix = glm::perspective(glm::radians((float)this->fov), window->props.aspectRatio, 0.01f, 200.0f);
    projectionMatrixMinimap = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 0.5f, 100.0f);

    this->color = glm::vec3(32.0f / 255, 32.0f / 255, 32.0f / 255);
    this->cameraOffset = glm::vec3(-1.0, 0.5f, 0.2f);
    this->initialVertices = {
        VertexFormat(glm::vec3(0, 0, 10), color),
        VertexFormat(glm::vec3(0.44109, 0, 9.90722), color),
        VertexFormat(glm::vec3(0.89002, 0, 9.7875), color),
        VertexFormat(glm::vec3(1.30901, 0, 9.60793), color),
        VertexFormat(glm::vec3(1.69808, 0, 9.33858), color),
        VertexFormat(glm::vec3(2.11707, 0, 9.00937), color),
        VertexFormat(glm::vec3(2.44628, 0, 8.6203), color),
        VertexFormat(glm::vec3(2.74556, 0, 8.20131), color),
        VertexFormat(glm::vec3(3.01492, 0, 7.75239), color),
        VertexFormat(glm::vec3(3.19449, 0, 7.27354), color),
        VertexFormat(glm::vec3(3.3142, 0, 6.79469), color),
        VertexFormat(glm::vec3(3.40398, 0, 6.34576), color),
        VertexFormat(glm::vec3(3.43391, 0, 5.80706), color),
        VertexFormat(glm::vec3(3.3142, 0, 5.29828), color),
        VertexFormat(glm::vec3(2.98499, 0, 4.81943), color),
        VertexFormat(glm::vec3(2.50614, 0, 4.58), color),
        VertexFormat(glm::vec3(2.02729, 0, 4.55007), color),
        VertexFormat(glm::vec3(1.54844, 0, 4.52014), color),
        VertexFormat(glm::vec3(1.09951, 0, 4.49022), color),
        VertexFormat(glm::vec3(0.59073, 0, 4.46029), color),
        VertexFormat(glm::vec3(0, 0, 4.46029), color),
        VertexFormat(glm::vec3(-0.60639, 0, 4.43036), color),
        VertexFormat(glm::vec3(-1.1451, 0, 4.40043), color),
        VertexFormat(glm::vec3(-1.62395, 0, 4.40043), color),
        VertexFormat(glm::vec3(-2.3123, 0, 4.3705), color),
        VertexFormat(glm::vec3(-2.91086, 0, 4.3705), color),
        VertexFormat(glm::vec3(-3.50943, 0, 4.40043), color),
        VertexFormat(glm::vec3(-4.07806, 0, 4.40043), color),
        VertexFormat(glm::vec3(-4.61677, 0, 4.3705), color),
        VertexFormat(glm::vec3(-5.15548, 0, 4.34057), color),
        VertexFormat(glm::vec3(-5.63433, 0, 4.34057), color),
        VertexFormat(glm::vec3(-6.17303, 0, 4.3705), color),
        VertexFormat(glm::vec3(-6.59203, 0, 4.3705), color),
        VertexFormat(glm::vec3(-7.13073, 0, 4.3705), color),
        VertexFormat(glm::vec3(-7.43002, 0, 4.04129), color),
        VertexFormat(glm::vec3(-7.7293, 0, 3.6223), color),
        VertexFormat(glm::vec3(-7.84901, 0, 3.05366), color),
        VertexFormat(glm::vec3(-7.75923, 0, 2.48503), color),
        VertexFormat(glm::vec3(-7.57966, 0, 1.97625), color),
        VertexFormat(glm::vec3(-7.37016, 0, 1.4974), color),
        VertexFormat(glm::vec3(-6.86138, 0, 1.10833), color),
        VertexFormat(glm::vec3(-6.47231, 0, 0.71927), color),
        VertexFormat(glm::vec3(-6.05332, 0, 0.3302), color),
        VertexFormat(glm::vec3(-5.66425, 0, 0), color),
        VertexFormat(glm::vec3(-5.21533, 0, -0.44793), color),
        VertexFormat(glm::vec3(-4.82627, 0, -0.837), color),
        VertexFormat(glm::vec3(-4.4372, 0, -1.19614), color),
        VertexFormat(glm::vec3(-4.04813, 0, -1.55528), color),
        VertexFormat(glm::vec3(-3.74885, 0, -2.03413), color),
        VertexFormat(glm::vec3(-3.62914, 0, -2.57283), color),
        VertexFormat(glm::vec3(-3.62914, 0, -3.1714), color),
        VertexFormat(glm::vec3(-3.68899, 0, -3.76996), color),
        VertexFormat(glm::vec3(-3.80871, 0, -4.42838), color),
        VertexFormat(glm::vec3(-3.71892, 0, -4.99702), color),
        VertexFormat(glm::vec3(-3.38971, 0, -5.5058), color),
        VertexFormat(glm::vec3(-2.94079, 0, -5.86493), color),
        VertexFormat(glm::vec3(-2.37215, 0, -6.01457), color),
        VertexFormat(glm::vec3(-1.74366, 0, -6.07443), color),
        VertexFormat(glm::vec3(-1.1451, 0, -6.07443), color),
        VertexFormat(glm::vec3(-0.57646, 0, -6.0445), color),
        VertexFormat(glm::vec3(0, 0, -6), color),
        VertexFormat(glm::vec3(0.59073, 0, -6.0445), color),
        VertexFormat(glm::vec3(1.1893, 0, -6.0445), color),
        VertexFormat(glm::vec3(2, 0, -6), color),
        VertexFormat(glm::vec3(2.71564, 0, -6.07443), color),
        VertexFormat(glm::vec3(3.34413, 0, -6.01457), color),
        VertexFormat(glm::vec3(4, 0, -6), color),
        VertexFormat(glm::vec3(4.57118, 0, -6.0445), color),
        VertexFormat(glm::vec3(5.10989, 0, -6.0445), color),
        VertexFormat(glm::vec3(5.55881, 0, -6.01457), color),
        VertexFormat(glm::vec3(6, 0, -6), color),
        VertexFormat(glm::vec3(6.51651, 0, -6.0445), color),
        VertexFormat(glm::vec3(7.02529, 0, -6.07443), color),
        VertexFormat(glm::vec3(7.53407, 0, -6.10436), color),
        VertexFormat(glm::vec3(8, 0, -6), color),
        VertexFormat(glm::vec3(8.58156, 0, -5.98465), color),
        VertexFormat(glm::vec3(9.09034, 0, -6.10436), color),
        VertexFormat(glm::vec3(9.41955, 0, -6.37371), color),
        VertexFormat(glm::vec3(9.77868, 0, -6.79271), color),
        VertexFormat(glm::vec3(10.07797, 0, -7.2117), color),
        VertexFormat(glm::vec3(10, 0, -8), color),
        VertexFormat(glm::vec3(9.80861, 0, -8.64825), color),
        VertexFormat(glm::vec3(9.4794, 0, -9.15703), color),
        VertexFormat(glm::vec3(9.06041, 0, -9.51617), color),
        VertexFormat(glm::vec3(8.67134, 0, -9.7556), color),
        VertexFormat(glm::vec3(8, 0, -10), color),
        VertexFormat(glm::vec3(7.47422, 0, -10.05488), color),
        VertexFormat(glm::vec3(6.84572, 0, -10.02495), color),
        VertexFormat(glm::vec3(6.30702, 0, -9.99502), color),
        VertexFormat(glm::vec3(5.70845, 0, -9.99502), color),
        VertexFormat(glm::vec3(5.13982, 0, -9.99502), color),
        VertexFormat(glm::vec3(4.66097, 0, -10.02495), color),
        VertexFormat(glm::vec3(4, 0, -10), color),
        VertexFormat(glm::vec3(3.43391, 0, -10.02495), color),
        VertexFormat(glm::vec3(2.80542, 0, -10.02495), color),
        VertexFormat(glm::vec3(2, 0, -10), color),
        VertexFormat(glm::vec3(1.3988, 0, -10.02495), color),
        VertexFormat(glm::vec3(0.83016, 0, -10.02495), color),
        VertexFormat(glm::vec3(0.26152, 0, -10.02495), color),
        VertexFormat(glm::vec3(-0.51661, 0, -10.02495), color),
        VertexFormat(glm::vec3(-1.11517, 0, -10.05488), color),
        VertexFormat(glm::vec3(-1.74366, 0, -10.05488), color),
        VertexFormat(glm::vec3(-2.43201, 0, -10.02495), color),
        VertexFormat(glm::vec3(-3.00065, 0, -10.05488), color),
        VertexFormat(glm::vec3(-3.56928, 0, -10.05488), color),
        VertexFormat(glm::vec3(-4, 0, -10), color),
        VertexFormat(glm::vec3(-4.67662, 0, -10.05488), color),
        VertexFormat(glm::vec3(-5.30512, 0, -9.99502), color),
        VertexFormat(glm::vec3(-6, 0, -10), color),
        VertexFormat(glm::vec3(-6.65188, 0, -9.96509), color),
        VertexFormat(glm::vec3(-7.5198, 0, -10.05488), color),
        VertexFormat(glm::vec3(-8, 0, -10), color),
        VertexFormat(glm::vec3(-8.65707, 0, -10.11473), color),
        VertexFormat(glm::vec3(-9.28556, 0, -9.90524), color),
        VertexFormat(glm::vec3(-9.55492, 0, -9.51617), color),
        VertexFormat(glm::vec3(-9.79434, 0, -8.94754), color),
        VertexFormat(glm::vec3(-9.91406, 0, -8.34897), color),
        VertexFormat(glm::vec3(-10.09362, 0, -7.4212), color),
        VertexFormat(glm::vec3(-10.36298, 0, -6.88249), color),
        VertexFormat(glm::vec3(-10.72212, 0, -6.43357), color),
        VertexFormat(glm::vec3(-11.08125, 0, -6.01457), color),
        VertexFormat(glm::vec3(-11.50025, 0, -5.56565), color),
        VertexFormat(glm::vec3(-11.7696, 0, -5.11673), color),
        VertexFormat(glm::vec3(-11.91924, 0, -4.60795), color),
        VertexFormat(glm::vec3(-12, 0, -4), color),
        VertexFormat(glm::vec3(-11.94917, 0, -3.32104), color),
        VertexFormat(glm::vec3(-12.03896, 0, -2.72248), color),
        VertexFormat(glm::vec3(-12, 0, -2), color),
        VertexFormat(glm::vec3(-12.12874, 0, -1.25599), color),
        VertexFormat(glm::vec3(-12.60759, 0, -0.74722), color),
        VertexFormat(glm::vec3(-13.11637, 0, -0.38808), color),
        VertexFormat(glm::vec3(-13.41565, 0, 0), color),
        VertexFormat(glm::vec3(-13.71493, 0, 0.47984), color),
        VertexFormat(glm::vec3(-13.8945, 0, 1.01855), color),
        VertexFormat(glm::vec3(-13.98429, 0, 1.61711), color),
        VertexFormat(glm::vec3(-13.98429, 0, 2.33539), color),
        VertexFormat(glm::vec3(-13.95436, 0, 2.99381), color),
        VertexFormat(glm::vec3(-14.01422, 0, 3.65223), color),
        VertexFormat(glm::vec3(-14.04414, 0, 4.49022), color),
        VertexFormat(glm::vec3(-14.01422, 0, 5.23842), color),
        VertexFormat(glm::vec3(-13.68501, 0, 5.80706), color),
        VertexFormat(glm::vec3(-13.02659, 0, 6.37569), color),
        VertexFormat(glm::vec3(-12.60759, 0, 6.82461), color),
        VertexFormat(glm::vec3(-12.06888, 0, 7.30346), color),
        VertexFormat(glm::vec3(-11.47032, 0, 7.75239), color),
        VertexFormat(glm::vec3(-10.66226, 0, 8.32102), color),
        VertexFormat(glm::vec3(-9.97391, 0, 8.79987), color),
        VertexFormat(glm::vec3(-9.55581, 0, 9.0951), color),
        VertexFormat(glm::vec3(-9.33886, 0, 9.28493), color),
        VertexFormat(glm::vec3(-9.17615, 0, 9.61034), color),
        VertexFormat(glm::vec3(-9.13548, 0, 10.00356), color),
        VertexFormat(glm::vec3(-9.14903, 0, 10.35609), color),
        VertexFormat(glm::vec3(-9.0948, 0, 10.78999), color),
        VertexFormat(glm::vec3(-8.89141, 0, 11.12896), color),
        VertexFormat(glm::vec3(-8.62023, 0, 11.30523), color),
        VertexFormat(glm::vec3(-8.28125, 0, 11.40015), color),
        VertexFormat(glm::vec3(-7.99651, 0, 11.44082), color),
        VertexFormat(glm::vec3(-7.76601, 0, 11.44082), color),
        VertexFormat(glm::vec3(-7.45415, 0, 11.46794), color),
        VertexFormat(glm::vec3(-7.04737, 0, 11.46794), color),
        VertexFormat(glm::vec3(-6.66772, 0, 11.50862), color),
        VertexFormat(glm::vec3(-6.38298, 0, 11.50862), color),
        VertexFormat(glm::vec3(-6.12535, 0, 11.52218), color),
        VertexFormat(glm::vec3(-5.79994, 0, 11.50862), color),
        VertexFormat(glm::vec3(-5.59655, 0, 11.42726), color),
        VertexFormat(glm::vec3(-5.40672, 0, 11.27811), color),
        VertexFormat(glm::vec3(-5.25757, 0, 11.04761), color),
        VertexFormat(glm::vec3(-5.13554, 0, 10.8171), color),
        VertexFormat(glm::vec3(-5.04063, 0, 10.55948), color),
        VertexFormat(glm::vec3(-4.94571, 0, 10.32898), color),
        VertexFormat(glm::vec3(-4.81012, 0, 10.04424), color),
        VertexFormat(glm::vec3(-4.64741, 0, 9.81373), color),
        VertexFormat(glm::vec3(-4.44403, 0, 9.70526), color),
        VertexFormat(glm::vec3(-4.14573, 0, 9.65102), color),
        VertexFormat(glm::vec3(-3.71183, 0, 9.65102), color),
        VertexFormat(glm::vec3(-3.31862, 0, 9.65102), color),
        VertexFormat(glm::vec3(-2.93897, 0, 9.65102), color),
        VertexFormat(glm::vec3(-2.72202, 0, 9.6917), color),
        VertexFormat(glm::vec3(-2.3966, 0, 9.75949), color),
        VertexFormat(glm::vec3(-1.97627, 0, 9.85441), color),
        VertexFormat(glm::vec3(-1.63729, 0, 9.94932), color),
        VertexFormat(glm::vec3(-1.24408, 0, 10.03068), color),
        VertexFormat(glm::vec3(-0.82375, 0, 10.05779), color),
        VertexFormat(glm::vec3(-0.48477, 0, 10.05779), color),
        VertexFormat(glm::vec3(-0.26782, 0, 10.04424), color)
    };

    /* The initial position and rotation of the car controled by the player */
    this->rotatePlayerCar = atan((initialVertices.at(1).position.x - initialVertices.at(0).position.x)
                                 / (initialVertices.at(1).position.z - initialVertices.at(0).position.z));
    this->carPosition = glm::vec3(this->initialVertices.at(0).position.x, this->initialVertices.at(0).position.y + 0.01f, this->initialVertices.at(0).position.z);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    /* The initial positions of the cameras */
    camera = new implemented::Camera();
    camera->Set(this->carPosition + this->cameraOffset, this->carPosition, glm::vec3(0, 1, 0));
    camera->distanceToTarget = distance(this->carPosition, camera->position);

    minimapCamera = new implemented::Camera();
    minimapCamera->Set(this->carPosition + glm::vec3(1, 5.0f, 0), this->carPosition, glm::vec3(0, 1, 0));
    minimapCamera->distanceToTarget = distance(this->carPosition, minimapCamera->position);

    /* The generation of the track and the creation of all the meshes that are going to be loaded
    on the screen */
    std::vector<VertexFormat> aboveVertices;
    std::vector<VertexFormat> belowVertices;

    track::generatePoints(glm::vec3(32.0f / 255, 32.0f / 255, 32.0f / 255), initialVertices, aboveVertices, belowVertices, 0.55f);
    Mesh* track = track::createTrack("track", aboveVertices, belowVertices, vertices, fewerVertices);
    AddMeshToList(track);

    Mesh *trackFrame = track::createTrackFrame("trackFrame", 1.0f, 1.0f, glm::vec3(0, 120.0f / 255, 0), GL_TRIANGLES);
    AddMeshToList(trackFrame);

    Mesh* baseOfTheTree = tree::createBaseOfTheTree("baseOfTheTree", glm::vec3(102.0f / 255, 51.0f / 255, 0));
    AddMeshToList(baseOfTheTree);

    Mesh* leaves = tree::createTree("leaves", glm::vec3(0, 80.0f / 255, 0));
    AddMeshToList(leaves);

    Mesh* secondLeaves = tree::createBaseOfTheTree("secondLeaves", glm::vec3(0, 80.0f / 255, 0));
    AddMeshToList(secondLeaves);

    Mesh* cars = tree::createBaseOfTheTree("cars", glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255));
    AddMeshToList(cars);

    Mesh* playerCar = tree::createBaseOfTheTree("playerCar", glm::vec3(51.0f / 255, 1.0f, 1.0f));
    AddMeshToList(playerCar);

    Mesh* startingLine = rectangle::CreateRectangle("startingLine", 0.1f, 1.1f, glm::vec3(1, 1, 1), GL_TRIANGLES);
    AddMeshToList(startingLine);

    track::generatePoints(glm::vec3(1, 1, 1), this->initialVertices, this->firstTreePositions, this->secondTreePositions, 1.5f);

    determineObstaclesTrack();

    // Create a shader program for the visual effect of the game
    {
        Shader* shader = new Shader("Tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* The initialization of the text rendered on the screen */
    this->resolution = window->GetResolution();
    this->currentText = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    this->currentText->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
}


void Tema2::FrameStart()
{
    this->resolution = window->GetResolution();
    
    // If the game is over we prind „Game Over” aka if the number of laps set were done
    if (MAX_LAPS - this->numberOfLaps <= 0) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, resolution.x, resolution.y);
        char textScore[100] = "Game Over";
        currentText->RenderText(textScore, (float) resolution.x / 4, (float)resolution.y / 3, 3.0f);
    }
    else {
        // Clears the color buffer (using the previously set color) and depth buffer
        glClearColor(0, 204.0f / 255, 204.0f / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sets the screen area where to draw
        glViewport(0, 0, resolution.x, resolution.y);
        this->lengthOfTheCar = (float)resolution.y / 4000;

        generateTrees(firstTreePositions, projectionMatrix, camera);
        generateTrees(secondTreePositions, projectionMatrix, camera);

        this->modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 0.015f, 0);
        RenderSimpleMesh(meshes["track"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);

        this->modelMatrix = glm::mat4(1);
        this->modelMatrix *= transform3D::Translate(this->initialVertices.at(0).position.x, 0.016f,
                                                    this->initialVertices.at(0).position.z);
        this->modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));
        this->modelMatrix *= transform3D::RotateOZ(glm::radians(-10.0f));
        RenderSimpleMesh(meshes["startingLine"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);
        
        /* The grass is a 40 x 40 square made by putting together one by another squares of dimension
        1 x 1 squares. This is made in order to have a high density of triangles*/
        for (int i = -20; i <= 20; i++) {
            for (int j = -20; j <= 20; j++) {
                this->modelMatrix = glm::mat4(1);
                this->modelMatrix *= transform3D::Translate(i, 0, j);
                RenderSimpleMesh(meshes["trackFrame"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);
            }
        }
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (MAX_LAPS - this->numberOfLaps > 0) {
        this->deltaTimeMinimap = deltaTimeSeconds;
        glLineWidth(3);
        glPointSize(5);
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        this->resolution = window->GetResolution();

        /* Determine the new positions for the enemies cars */
        determineTranslationPosition(deltaTimeSeconds);

        /* Update the position of the enemies */
        for (int i = 0; i < this->numberOfCars; i++) {
            this->modelMatrix = glm::mat4(1);
            this->modelMatrix *= transform3D::Translate(0, 0.1f, 0);
            this->modelMatrix *= transform3D::Translate(this->translationPositions.at(i).x, 0.016f, this->translationPositions.at(i).z);
            this->modelMatrix *= transform3D::RotateOY(this->rotationPositions.at(i));
            this->modelMatrix *= transform3D::Scale((float)resolution.x / 15000, 0.1f, (float)resolution.y / 4000);
            RenderSimpleMesh(meshes["cars"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);
        }

        /* Update the position of the player's car */
        this->modelMatrix = glm::mat4(1);
        this->modelMatrix *= transform3D::Translate(0, 0.1f, 0);
        this->modelMatrix *= transform3D::Translate(carPosition.x, carPosition.y, carPosition.z);
        this->modelMatrix *= transform3D::RotateOY(this->rotatePlayerCar);
        this->modelMatrix *= transform3D::Scale((float)resolution.x / 15000, 0.1f, (float)resolution.y / 4000);
        RenderSimpleMesh(meshes["playerCar"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);

        // Check if another lap was completed
        lapDoneFunction(carPosition, deltaTimeSeconds);

        // Render the text specific to the number of laps or the time remaining
        char lapsText[100];
        sprintf(lapsText, "Number of laps done: %d", this->numberOfLaps);
        this->currentText->RenderText(lapsText, (float)resolution.x / 80, (float)resolution.y / 80, 1.0f, glm::vec3(1, 1, 1));

        char timeSinceLastLap[100];
        sprintf(timeSinceLastLap, "Time since last lap: %f", this->timeSinceLastLap);
        this->currentText->RenderText(timeSinceLastLap, (float)resolution.x / 80, (float)resolution.y / 22, 1.0f, glm::vec3(1, 1, 1));

        char numberOfLapsRemaining[100];
        sprintf(numberOfLapsRemaining, "Number of laps remaining: %d", MAX_LAPS - this->numberOfLaps);
        this->currentText->RenderText(numberOfLapsRemaining, (float)resolution.x / 80, (float)resolution.y / 12, 1.0f, glm::vec3(1, 1, 1));
    }
}


void Tema2::FrameEnd()
{
    // The rendering of the same scene again with a different projection matrix for the minimap
    if (MAX_LAPS - this->numberOfLaps > 0) {
        this->resolution = window->GetResolution();
        glViewport(10, 10, resolution.x / 5, resolution.y / 5);
        glLineWidth(3);
        glPointSize(5);
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        this->modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 0.015f, 0);
        RenderSimpleMesh(meshes["track"], shaders["Tema2Shader"], this->modelMatrix, minimapCamera, projectionMatrixMinimap);

        this->modelMatrix = glm::mat4(1);
        this->modelMatrix *= transform3D::Translate(this->initialVertices.at(0).position.x, 0.016f,
            this->initialVertices.at(0).position.z);
        this->modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));
        this->modelMatrix *= transform3D::RotateOZ(glm::radians(-10.0f));
        RenderSimpleMesh(meshes["startingLine"], shaders["Tema2Shader"], this->modelMatrix, minimapCamera, projectionMatrixMinimap);

        for (int i = -20; i <= 20; i++) {
            for (int j = -20; j <= 20; j++) {
                this->modelMatrix = glm::mat4(1);
                this->modelMatrix *= transform3D::Translate(i, 0, j);
                RenderSimpleMesh(meshes["trackFrame"], shaders["Tema2Shader"], this->modelMatrix, minimapCamera, projectionMatrixMinimap);
            }
        }

        generateTrees(firstTreePositions, projectionMatrixMinimap, minimapCamera);
        generateTrees(secondTreePositions, projectionMatrixMinimap, minimapCamera);

        determineTranslationPosition(deltaTimeMinimap);

        for (int i = 0; i < this->numberOfCars; i++) {
            this->modelMatrix = glm::mat4(1);
            this->modelMatrix *= transform3D::Translate(0, 0.1f, 0);
            this->modelMatrix *= transform3D::Translate(this->translationPositions.at(i).x, 0.016f, this->translationPositions.at(i).z);
            this->modelMatrix *= transform3D::RotateOY(this->rotationPositions.at(i));
            this->modelMatrix *= transform3D::Scale((float)resolution.x / 15000, 0.1f, (float)resolution.y / 4000);
            RenderSimpleMesh(meshes["cars"], shaders["Tema2Shader"], this->modelMatrix, minimapCamera, projectionMatrixMinimap);
        }

        this->modelMatrix = glm::mat4(1);
        this->modelMatrix *= transform3D::Translate(0, 0.1f, 0);
        this->modelMatrix *= transform3D::Translate(carPosition.x, carPosition.y, carPosition.z);
        this->modelMatrix *= transform3D::RotateOY(this->rotatePlayerCar);
        this->modelMatrix *= transform3D::Scale((float)resolution.x / 15000, 0.1f, (float)resolution.y / 4000);
        RenderSimpleMesh(meshes["playerCar"], shaders["Tema2Shader"], this->modelMatrix, minimapCamera, projectionMatrixMinimap);

        minimapCamera->Set(this->carPosition + glm::vec3(1, 5.0f, 0), this->carPosition, glm::vec3(0, 1, 0));
        minimapCamera->distanceToTarget = distance(this->carPosition, minimapCamera->position);
    }
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // The logic of the translation of the car (checking for collisions with the enemies
    // or check whether it goes outside the track or not
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float translationFactor = 2.0f;
        this->collisionDetected = false;
        glm::vec3 newPoint = this->carPosition;

        if (window->KeyHold(GLFW_KEY_W)) {
            newPoint += glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S)) {
            newPoint -= glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;
        }

        for (int carNumber = 0; carNumber < this->numberOfCars; carNumber++) {
            if (collision::determineDinamicCollision(carNumber, this->lengthOfTheCar / 2, this->translationPositions, newPoint)) {
                this->collisionDetected = true;
                return;
            }
        }

        if (this->collisionDetected == false) {
            if (window->KeyHold(GLFW_KEY_A)) {
                float rotationFactor = 1.5f;
                this->rotatePlayerCar += rotationFactor * deltaTime;
                camera->RotateThirdPerson_OY(-rotationFactor * deltaTime);
            }
            else if (window->KeyHold(GLFW_KEY_D)) {
                float rotationFactor = 1.5f;
                this->rotatePlayerCar -= rotationFactor * deltaTime;
                camera->RotateThirdPerson_OY(rotationFactor * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_W)) {
                float translationFactor = 2.5f;
                glm::vec3 newPoint = this->carPosition + glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;

                for (int i = 0; i < fewerVertices.size(); i += 3) {
                    if (collision::insideTriangle(newPoint, fewerVertices.at(i).position, fewerVertices.at(i + 1).position, fewerVertices.at(i + 2).position)) {
                        this->carPosition += glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;
                        camera->MoveForward(translationFactor * deltaTime);
                        break;
                    }
                }
            }
            else if (window->KeyHold(GLFW_KEY_S)) {
                float translationFactor = 2.5f;
                glm::vec3 newPoint = this->carPosition - glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;

                for (int i = 0; i < fewerVertices.size(); i += 3) {
                    if (collision::insideTriangle(newPoint, fewerVertices.at(i).position, fewerVertices.at(i + 1).position, fewerVertices.at(i + 2).position)) {
                        this->carPosition -= glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * translationFactor * deltaTime;
                        camera->MoveForward(-translationFactor * deltaTime);
                        break;
                    }
                }
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

/* The rendering function used for rendering meshes and sending data to the shaders */
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
                             implemented::Camera *currentCamera, glm::mat4& currentProjectionMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = currentCamera->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(currentProjectionMatrix));

    int carPositionLocation = glGetUniformLocation(shader->program, "carPosition");
    glUniform3f(carPositionLocation, carPosition.x, carPosition.y, carPosition.z);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::determineObstaclesTrack() {
    std::vector<VertexFormat> firstTrack, secondTrack, thirdTrack, fourthTrack;
    track::generatePoints(glm::vec3(1, 1, 1), initialVertices, firstTrack, secondTrack, 0.1f);
    track::generatePoints(glm::vec3(1, 1, 1), initialVertices, thirdTrack, fourthTrack, 0.25f);

    this->tracks.push_back(firstTrack);
    this->tracks.push_back(secondTrack);
    this->tracks.push_back(thirdTrack);
    this->tracks.push_back(fourthTrack);

    for (int i = 0; i < this->numberOfCars; i++) {
        this->translationPositions.at(i) = tracks.at(i).at(i).position;

        this->rotationPositions.at(i) = atan((tracks.at(i).at(i + 1).position.x - tracks.at(i).at(i).position.x) /
                                              (tracks.at(i).at(i + 1).position.z - tracks.at(i).at(i).position.z));
    }
}

void Tema2::determineTranslationPosition(float deltaTimeSeconds) {
    for (int i = 0; i < this->numberOfCars; i++) {
        if (this->currentVertex.at(i) + 1 == this->tracks.at(i).size()) {
            glm::vec3 firstPoint = tracks.at(i).at(this->currentVertex.at(i)).position;
            glm::vec3 secondPoint = tracks.at(i).at(0).position;

            this->rotationPositions.at(i) = atan((firstPoint.x - secondPoint.x) / (firstPoint.z - secondPoint.z));
            this->translationPositions.at(i).x += (firstPoint.x - secondPoint.x) * deltaTimeSeconds * this->speed / (i + 1);
            this->translationPositions.at(i).z += (firstPoint.z - secondPoint.z) * deltaTimeSeconds * this->speed / (i + 1);

            if (distance(this->translationPositions.at(i), tracks.at(i).at(this->currentVertex.at(i)).position)
                > distance(tracks.at(i).at(0).position, tracks.at(i).at(this->currentVertex.at(i)).position)) {
                this->currentVertex.at(i) = 0;
                this->translationPositions.at(i) = tracks.at(i).at(0).position;
            }

        }
        else {
            glm::vec3 firstPoint = tracks.at(i).at(this->currentVertex.at(i) + 1).position;
            glm::vec3 secondPoint = tracks.at(i).at(this->currentVertex.at(i)).position;

            this->rotationPositions.at(i) = atan((firstPoint.x - secondPoint.x) / (firstPoint.z - secondPoint.z));
            this->translationPositions.at(i).x += (firstPoint.x - secondPoint.x) * deltaTimeSeconds * this->speed / (i + 1);
            this->translationPositions.at(i).z += (firstPoint.z - secondPoint.z) * deltaTimeSeconds * this->speed / (i + 1);
            
            if (distance(this->translationPositions.at(i), tracks.at(i).at(this->currentVertex.at(i)).position)
                > distance(tracks.at(i).at(this->currentVertex.at(i) + 1).position, tracks.at(i).at(this->currentVertex.at(i)).position)) {
                this->translationPositions.at(i) = tracks.at(i).at(this->currentVertex.at(i) + 1).position;
                this->currentVertex.at(i)++;
            }
        }
    }
}

void Tema2::generateTrees(std::vector<VertexFormat> treePositions, glm::mat4 projectionMatrix, implemented::Camera *camera) {
    for (int i = 0; i < treePositions.size(); i += 5) {
        glm::vec3 treePosition = treePositions.at(i).position;
        if (i != 0) {
            this->modelMatrix = glm::mat4(1);
            this->modelMatrix *= transform3D::Translate(treePosition.x, 0, treePosition.z);
            this->modelMatrix *= transform3D::Translate(0, 0.75f, 0);
            this->modelMatrix *= transform3D::Scale(0.35f, 0.75f, 0.35f);
            RenderSimpleMesh(meshes["baseOfTheTree"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);

            if (this->numberOfTrees % 2 == 0) {
                this->modelMatrix = glm::mat4(1);
                this->modelMatrix *= transform3D::Translate(treePosition.x, 0, treePosition.z);
                this->modelMatrix *= transform3D::Translate(0, 1 + 0.75f * 2, 0);
                RenderSimpleMesh(meshes["leaves"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);

            }
            else {
                this->modelMatrix = glm::mat4(1);
                this->modelMatrix *= transform3D::Translate(treePosition.x, 0, treePosition.z);
                this->modelMatrix *= transform3D::Translate(0, 0.75f * 3, 0);
                this->modelMatrix *= transform3D::Scale(0.85f, 0.75f, 0.85f);
                RenderSimpleMesh(meshes["secondLeaves"], shaders["Tema2Shader"], this->modelMatrix, camera, projectionMatrix);
            }

            this->numberOfTrees++;
        }
    }
}

void Tema2::lapDoneFunction(glm::vec3& carPosition, float deltaTimeSeconds) {
    if (this->halfLapDone == false) {
        if (distance(carPosition, this->fewerVertices.at(this->fewerVertices.size() / 2).position) < 1.1f) {
            this->halfLapDone = true;
        }
    }

    if (this->carPosition.x <= this->fewerVertices.at(0).position.x && this->carPosition.x >= this->fewerVertices.at(1).position.x
        && this->carPosition.z <= this->fewerVertices.at(0).position.z && this->carPosition.z >= this->fewerVertices.at(1).position.z) {
        
        if (this->lapDone == true && this->halfLapDone == true) {
            this->timeSinceLastLap = 0;
            this->numberOfLaps++;

            this->lapDone = false;
            this->halfLapDone = false;
        }
    }
    else {
        this->lapDone = true;
        this->timeSinceLastLap += deltaTimeSeconds;
    }
}

