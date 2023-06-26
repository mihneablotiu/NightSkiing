#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace collision
{
    float triangleArea(glm::vec3 firstVertex, glm::vec3 secondVertex, glm::vec3 thirdVertex);
    bool insideTriangle(glm::vec3 point, glm::vec3 firstVertex, glm::vec3 secondVertex, glm::vec3 thirdVertex);
    bool determineDinamicCollision(int carNumber, float lengthOfTheCar, std::vector<glm::vec3> translationPositions, glm::vec3 carPosition);
}