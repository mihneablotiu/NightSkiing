#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace createCircle
{
    Mesh* CreateMyCircle(const std::string& name, float radius, float fragments, glm::vec3 color);
}