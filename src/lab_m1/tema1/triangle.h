#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace triangle
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateTriangle(const std::string& name, float length, glm::vec3 color);
}