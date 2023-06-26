#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/triangle.h"
#include "lab_m1/tema1/createCircle.h"
#include "lab_m1/tema1/createRectangle.h"
#include "components/text_renderer.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
    /* The initialization of all the parameters */
    this->angularStepWings = 0;
    this->translateX = 0;
    this->translateY = 0;

    this->isMaximumWings = false;
    this->isADuckSpawned = false;
    this->duckWasHit = false;
    this->firstMiss = this->secondMiss = this->thirdMiss = false;
    this->firstBullet = this->secondBullet = this->thirdBullet = false;
    this->resolution = window->GetResolution();
    this->finishAnimation = true;

    this->outsideScreenHorizontal = 0;
    this->outsideScreenVertical = 0;
    this->timeSpentForADuck = 0;
    this->lengthOfTheDuck = 0;
    this->widthOfTheDuck = 0;
    this->numberOfDucks = 0;
    this->initialSpeed = 200;
    this->bulletsShot = 0;
    this->score = 0;
    this->multiplierTime = 0;
    this->multiplierAngle = 0;
    this->multiplier = 1;
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    /* The creation of the meshes */

    /* The first life */
    Mesh* firstLife = createCircle::CreateMyCircle("firstLife", 15.0f, 10000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    AddMeshToList(firstLife);

    /* The second life */
    Mesh* secondLife = createCircle::CreateMyCircle("secondLife", 15.0f, 10000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    AddMeshToList(secondLife);

    /* The third life */
    Mesh* thirdLife = createCircle::CreateMyCircle("thirdLife", 15.0f, 10000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    AddMeshToList(thirdLife);

    /* The body of the duck */
    Mesh *body = triangle::CreateTriangle("body", 50.0f, glm::vec3(139.0f / 255, 69.0f / 255, 19.0f / 255));
    AddMeshToList(body);

    /* The beak of the duck */
    Mesh* beak = triangle::CreateTriangle("beak", 15.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    AddMeshToList(beak);

    /* The eye of the duck */
    Mesh* eye = createCircle::CreateMyCircle("eye", 5.0f, 10000.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(eye);

    /* The first bullet */
    Mesh* firstBullet = rectangle::CreateRectangle("firstBullet", 15.0f, 25.0f, glm::vec3(0.0f, 1.0f, 0.0f), GL_TRIANGLES);
    AddMeshToList(firstBullet);

    /* The second bullet */
    Mesh* secondBullet = rectangle::CreateRectangle("secondBullet", 15.0f, 25.0f, glm::vec3(0.0f, 1.0f, 0.0f), GL_TRIANGLES);
    AddMeshToList(secondBullet);

    /* The third bullet */
    Mesh* thirdBullet = rectangle::CreateRectangle("thirdBullet", 15.0f, 25.0f, glm::vec3(0.0f, 1.0f, 0.0f), GL_TRIANGLES);
    AddMeshToList(thirdBullet);

    /* The wireframe for the score */
    Mesh* wireframe = rectangle::CreateRectangle("wireframe", 200.0f, 30.0f, glm::vec3(1.0f, 1.0f, 1.0f), GL_LINE_LOOP);
    AddMeshToList(wireframe);

    /* The actual score */
    Mesh* score = rectangle::CreateRectangle("score", 10.f, 25.0f, glm::vec3(0.0f, 0.0f, 1.0f), GL_TRIANGLES);
    AddMeshToList(score);

    /* The grass for the bonus */
    Mesh* grass = rectangle::CreateRectangle("grass", resolution.x, 60.0f, glm::vec3(0.0f, 1.0f, 0.0f), GL_TRIANGLES);
    AddMeshToList(grass);

    Mesh* multiplier = rectangle::CreateRectangle("multiplier", 170.0f, 170.0f, glm::vec3(0.0f, 1.0f, 0.0f), GL_LINE_LOOP);
    AddMeshToList(multiplier);

    /* The initialization of the length and the width of the duck according to the dimensions of creating */
    this->lengthOfTheDuck = 50.0f + 40.0f + 15.0f;
    this->widthOfTheDuck = 50.0f;

    /* The initialization of the text renderer according to the ocw tutorial */
    this->currentText = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    this->currentText->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // The animation for the multiplier
    if (this->finishAnimation == false) {
        // The text for the multiplier
        this->multiplierString = "Multiplier: ";
        char multiplierValue[100];
        sprintf(multiplierValue, "%d", this->multiplier);
        this->multiplierString.append(multiplierValue);
        currentText->RenderText(this->multiplierString, (float)resolution.x / 2 - 70.0f, (float)resolution.y / 2 - 20, 1.0f);

        // The rotating square for the multiplier
        this->modelMatrix = glm::mat3(1);
        this->modelMatrix *= transform2D::Translate((float)resolution.x / 2, (float)resolution.y / 2);
        this->modelMatrix *= transform2D::Rotate(this->multiplierAngle);
        RenderMesh2D(meshes["multiplier"], shaders["VertexColor"], modelMatrix);
    }
    else {

        // If there is still one more life available (aka the game has not ended)
        if (this->thirdMiss == false) {
            // We always render the grass, the wireframe, the actual score and the
            // score text (these are permanent elements on the screen)
            this->modelMatrix = glm::mat3(1);
            this->modelMatrix *= transform2D::Translate((float)resolution.x / 2, 30.0f);
            RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

            this->modelMatrix = glm::mat3(1);
            this->modelMatrix *= transform2D::Translate(resolution.x - 125, resolution.y - 50);
            RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);

            this->modelMatrix = glm::mat3(1);
            this->modelMatrix *= transform2D::Translate(resolution.x - 225, resolution.y - 50);
            this->modelMatrix *= transform2D::Translate(this->score, 0.0f);
            this->modelMatrix *= transform2D::Scale((float)this->score / 5, 1.0f);
            RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

            // The render of the text score according to the tutorial
            this->textScore = "Score: ";
            char myScore[100];
            sprintf(myScore, "%d", this->score);
            this->textScore.append(myScore);
            currentText->RenderText(this->textScore, resolution.x - 200, 85, 1.5f);

            // If the game is still going on and there is not a duck on the screen right now
            // we have to spawn one
            if (this->isADuckSpawned == false) {
                // We count the number of ducks spawned already because we grow the speed from
                // 5 to 5 ducks
                this->numberOfDucks++;
                if ((this->numberOfDucks - 1) % 5 == 0) {
                    this->initialSpeed += this->initialSpeed / 3;

                    if (this->numberOfDucks != 1) {
                        this->finishAnimation = false;
                        this->multiplier++;
                    }
                }

                // We chose a random X position of the new duck and a random starting angle
                this->translateX = rand() % (this->resolution.x - 100);
                this->translateY = 0;
                this->modelMatrix = glm::mat3(1);
                randomStart = rand() % 60;
                if (randomStart <= 5) {
                    randomStart = 30;
                }

                randomStart = randomStart * M_PI / 180;

                // We reset the bullets for each new spawned duck
                this->isADuckSpawned = true;
                this->firstBullet = false;
                this->secondBullet = false;
                this->thirdBullet = false;
                this->bulletsShot = 0;

                // We create the head, left wing and right wing with new random colors
                this->randomColorHead = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
                Mesh* head = createCircle::CreateMyCircle("head", 20.0f, 10000.0f, this->randomColorHead);
                AddMeshToList(head);

                this->randomColorWings = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
                Mesh* leftWing = triangle::CreateTriangle("leftWing", 25.0f, this->randomColorWings);
                AddMeshToList(leftWing);

                Mesh* rightWing = triangle::CreateTriangle("rightWing", 25.0f, this->randomColorWings);
                AddMeshToList(rightWing);

            }
            // else aka there is still a duck on the screen right now so we don't spawn a new one
            else {
                // Create the moving matrix (aka the matrix that we apply on each element for the 
                // overall move of the duck. First of all we translate to the center of the duck,
                // then we rotate it if necesary (if there was a collision with the corespondent angle)
                // and the we translate it to the current position
                this->movingMatrix = transform2D::Translate(translateX, translateY);
                this->movingMatrix *= transform2D::Rotate(randomStart);
                this->movingMatrix *= transform2D::Translate(-lengthOfTheDuck / 2, -widthOfTheDuck / 2);

                // We apply individual transformations for each element and the we apply the moving
                // matrix for each of them
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(95.0f, 50.0f);
                RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(85.0f, 45.0f);
                RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(15.0f, 45.0f);
                RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(50.0f, 50.0f);
                this->modelMatrix *= transform2D::Rotate(1.50f);
                this->modelMatrix *= transform2D::Rotate(angularStepWings);
                RenderMesh2D(meshes["leftWing"], shaders["VertexColor"], modelMatrix);

                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(50.0f, 40.0f);
                this->modelMatrix *= transform2D::Rotate(-1.50f);
                this->modelMatrix *= transform2D::Rotate(-angularStepWings);
                RenderMesh2D(meshes["rightWing"], shaders["VertexColor"], modelMatrix);

                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= this->movingMatrix;
                this->modelMatrix *= transform2D::Translate(95.0f, 35.0f);
                this->modelMatrix *= transform2D::Rotate(-0.25f);
                RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
            }

            // We render just the bullets and the lifes that are still available
            // These are overall elements (they have nothing to do with individual ducks)
            if (this->thirdMiss == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(50, resolution.y - 50);
                RenderMesh2D(meshes["firstLife"], shaders["VertexColor"], modelMatrix);
            }

            if (this->secondMiss == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(100, resolution.y - 50);
                RenderMesh2D(meshes["secondLife"], shaders["VertexColor"], modelMatrix);
            }

            if (this->firstMiss == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(150, resolution.y - 50);
                RenderMesh2D(meshes["thirdLife"], shaders["VertexColor"], modelMatrix);
            }

            if (this->thirdBullet == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(48, resolution.y - 100);
                RenderMesh2D(meshes["firstBullet"], shaders["VertexColor"], modelMatrix);
            }

            if (this->secondBullet == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(98, resolution.y - 100);
                RenderMesh2D(meshes["secondBullet"], shaders["VertexColor"], modelMatrix);
            }

            if (this->firstBullet == false) {
                this->modelMatrix = glm::mat3(1);
                this->modelMatrix *= transform2D::Translate(148, resolution.y - 100);
                RenderMesh2D(meshes["thirdBullet"], shaders["VertexColor"], modelMatrix);
            }

            // Else means the game is over
        }
        else {

            // We just print on the screen a new text element notifing the player that the game is over
            this->textScore = "Game Over";
            currentText->RenderText(this->textScore, (float)resolution.x / 2 - 150, (float)resolution.y / 2 - 50, 3.0f);
        }
    }
}


void Tema1::Update(float deltaTimeSeconds)
{
    // The update of the multiplier animation
    if (this->finishAnimation == false) {
        this->multiplierAngle += deltaTimeSeconds;
        this->multiplierTime += deltaTimeSeconds;
        if (this->multiplierTime > 3.0f) {
            this->finishAnimation = true;
            this->multiplierAngle = 0;
            this->multiplierTime = 0;
        }
    }
    else {
        // The wings animation logic (we rotate on one direction until a limit
        // and then we change the direction of the rotation)
        if (isMaximumWings == false) {
            this->angularStepWings += deltaTimeSeconds;
            if (this->angularStepWings > 0.349f) {
                isMaximumWings = true;
            }
        }
        else {
            this->angularStepWings -= deltaTimeSeconds;
            if (this->angularStepWings <= 0) {
                isMaximumWings = false;
            }
        }

        // If a duck was hit with a bullet we just translate it downwards and when it
        // goes outside of the screen we just notify that we can spawn a new duck
        if (this->duckWasHit == true) {
            this->translateY += glm::sin(-M_PI / 2) * deltaTimeSeconds * initialSpeed;
            this->randomStart = -M_PI / 2;

            if (this->translateY < 0.0f) {
                this->timeSpentForADuck = 0;
                this->isADuckSpawned = false;
                this->duckWasHit = false;
            }
        }

        // If a the duck was not hit we have two choices
        else {

            // First of them is if the duck exceeded its time we have to let it escape or if we missed it with all of the three
            // bullets, it still has to escape
            if (this->timeSpentForADuck > 7.0f || (this->firstBullet == true && this->secondBullet == true && this->thirdBullet == true && duckWasHit == false)) {
                this->translateY += glm::sin(M_PI / 2) * deltaTimeSeconds * initialSpeed;
                this->randomStart = M_PI / 2;

                // After it escapes we have to check which life we have to remove
                if (this->translateY > this->resolution.y) {
                    this->timeSpentForADuck = 0;
                    this->isADuckSpawned = false;

                    if (this->thirdMiss == false && this->secondMiss == true && this->firstMiss == true) {
                        this->thirdMiss = true;
                    }
                    else if (this->secondMiss == false && this->firstMiss == true) {
                        this->secondMiss = true;
                    }
                    else if (this->firstMiss == false) {
                        this->firstMiss = true;
                    }
                }
            }

            // Or it still has time so it has to fly on its direction
            else {
                this->translateX += glm::cos(randomStart) * deltaTimeSeconds * initialSpeed;
                this->translateY += glm::sin(randomStart) * deltaTimeSeconds * initialSpeed;

                this->outsideScreenHorizontal = translateX;
                this->outsideScreenVertical = translateY;
                this->resolution = window->GetResolution();

                // We check if there is a collision of the beak with the margin of the screen
                // and if there is a collision then we change the rotation angle for the reflexion to happen
                if (this->outsideScreenHorizontal + meshes["beak"]->vertices.at(2).position.x > resolution.x) {
                    this->randomStart = M_PI - this->randomStart;
                }
                else if (this->outsideScreenHorizontal + meshes["beak"]->vertices.at(2).position.x < 0.0f) {
                    this->randomStart = 3 * M_PI - this->randomStart;
                }
                else if (this->outsideScreenVertical + meshes["beak"]->vertices.at(2).position.y > resolution.y) {
                    this->randomStart = -this->randomStart;
                }
                else if (this->outsideScreenVertical + meshes["beak"]->vertices.at(2).position.y < 0.0f) {
                    this->randomStart = -this->randomStart;
                }

                // Then we increment the time spent for a duck with deltaTimeSeconds
                this->timeSpentForADuck += deltaTimeSeconds;
            }
        }
    }
}


void Tema1::FrameEnd()
{
    
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Here we check if we hit the duck with the bullet or not. We just make a square of the length
    // lengthOfTheDuck / 2 starting from the center of the duck
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (this->myMouseX >= translateX - lengthOfTheDuck / 2 && this->myMouseX <= translateX + lengthOfTheDuck / 2
            && this->myMouseY >= translateY - lengthOfTheDuck / 2 && this->myMouseY <= translateY + lengthOfTheDuck / 2
            && this->timeSpentForADuck < 7.0f && this->bulletsShot < 3) {
            this->duckWasHit = true;

        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // We make the variable change operation because the normal coordinate system
    // starts in left bottom of the screen but the coordinates of the mouse start
    // on top left of the screen
    this->myMouseY = resolution.y - mouseY;
    this->myMouseX = mouseX;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // We do the number of bullets counting
    if (button == 1 && this->bulletsShot < 3) {
        if (this->firstBullet == false) {
            this->firstBullet = true;
        }
        else if (this->secondBullet == false) {
            this->secondBullet = true;
        }
        else if (this->thirdBullet == false) {
            this->thirdBullet = true;
            return;
        }

        this->bulletsShot++;
    }

    // Each time we shoot and it happens to hit the duck we modify the score accordingly
    if (button == 1 && this->bulletsShot < 3) {
        if (this->myMouseX >= translateX - lengthOfTheDuck / 2 && this->myMouseX <= translateX + lengthOfTheDuck / 2
            && this->myMouseY >= translateY - lengthOfTheDuck / 2 && this->myMouseY <= translateY + lengthOfTheDuck / 2
            && this->timeSpentForADuck < 7.0f) {
            
            if (this->score < 100) {
                this->score += this->multiplier;
            }

        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
