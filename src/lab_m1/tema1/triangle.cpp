#include "triangle.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* triangle::CreateTriangle(
    const std::string& name,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0,  -length / 2, 0), color),
        VertexFormat(corner + glm::vec3(0, length / 2, 0), color),
        VertexFormat(corner + glm::vec3(1.5 * length, 0, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle->InitFromData(vertices, indices);
    return triangle;
}
