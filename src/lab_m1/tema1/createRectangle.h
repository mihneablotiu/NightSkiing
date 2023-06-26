#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace rectangle
{
    Mesh* CreateRectangle(const std::string& name, float length, float width, glm::vec3 color, int drawMode);
}
