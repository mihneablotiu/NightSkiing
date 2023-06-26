#include "createRectangle.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* rectangle::CreateRectangle(
    const std::string& name,
    float length,
    float width,
    glm::vec3 color,
    int drawMode)
{
    glm::vec3 corner = glm::vec3(-length / 2, -width / 2, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, width, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    rectangle->SetDrawMode(drawMode);
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}
