#include "tree.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

/* The function that creates a cube used for the base
of the trees and for the threes that have the leaves as 
a cube and returns the mesh*/
Mesh* tree::createBaseOfTheTree(
    const std::string& name,
    glm::vec3 color
)
{
    std::vector<VertexFormat> vertices = 
    {
        VertexFormat(glm::vec3(-1, -1,  1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, -1,  1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, 1,  1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, 1,  1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, -1,  -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, -1,  -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, 1,  -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, 1,  -1), color, glm::vec3(0.2, 0.8, 0.6))
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,    // indices for first triangle
        1, 3, 2,    // indices for second triangle
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4

    };


    Mesh* base = new Mesh(name);
    base->SetDrawMode(GL_TRIANGLES);
    base->InitFromData(vertices, indices);

    return base;
}

/* The function that creates a piramyd for the trees that
use a piramyd for the leaves and returns the new created
mesh */
Mesh* tree::createTree(
    const std::string& name,
    glm::vec3 color
)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 1,  0), color, glm::vec3(0.2, 0.8, 0.6)), // the V of the pyramid
        VertexFormat(glm::vec3(-1, -1,  1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, -1,  -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, -1,  -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, -1,  1), color, glm::vec3(0.2, 0.8, 0.6))
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 4,
        2, 3, 4,
    };


    Mesh* leaves = new Mesh(name);
    leaves->SetDrawMode(GL_TRIANGLES);
    leaves->InitFromData(vertices, indices);

    return leaves;

}