#include "createCircle.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* createCircle::CreateMyCircle(
    const std::string& name,
    float radius,
    float fragments,
    glm::vec3 color)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    vertices.push_back(VertexFormat(corner, color));

    Mesh* circle = new Mesh(name);

    float increment = 2.0f * M_PI / fragments;
    int i = 1;

    circle->SetDrawMode(GL_LINE_LOOP);
    for (float currentAngle = 0.0f; currentAngle <= 2.0f * M_PI; currentAngle += increment) {
        glm::vec3 currentPosition = glm::vec3(radius * cos(currentAngle), radius * sin(currentAngle), 0);
        VertexFormat currentVertex = VertexFormat(currentPosition + corner, color);
        vertices.push_back(currentVertex);

        if (i % 3 == 0) {
            indices.push_back(i++);
            indices.push_back(0);
        }
        else {
            indices.push_back(i++);
        }
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
