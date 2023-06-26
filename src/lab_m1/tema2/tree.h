#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace tree
{
    Mesh* createBaseOfTheTree(const std::string& name, glm::vec3 color);
    Mesh* createTree(const std::string& name, glm::vec3 color);
}