#include "track.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

/* The function that generates points to the left and right
of a given set of points at a specific distance */
void track::generatePoints(
    glm::vec3 color,
    std::vector<VertexFormat> initialVertices,
    std::vector<VertexFormat> &aboveVertices,
    std::vector<VertexFormat> &belowVertices,
    float distanceUpAndDown) 
{
    glm::vec3 distanceUp = glm::vec3(distanceUpAndDown, 0, distanceUpAndDown);
    glm::vec3 distanceDown = glm::vec3(distanceUpAndDown, 0, distanceUpAndDown);

    for (int i = 0, j = i + 1; i < initialVertices.size() - 1; i++, j++) {
        VertexFormat firstPoint = initialVertices.at(i);
        VertexFormat secondPoint = initialVertices.at(j);
        glm::vec3 directionVector = glm::normalize(glm::vec3(secondPoint.position.x - firstPoint.position.x,
                                              0, secondPoint.position.z - firstPoint.position.z));

        glm::vec3 perpendicularVector = glm::cross(directionVector, glm::vec3(0, 1, 0));

        aboveVertices.push_back(VertexFormat(glm::vec3(glm::vec3(firstPoint.position.x, 0, firstPoint.position.z) + distanceUp * perpendicularVector), color));
        belowVertices.push_back(VertexFormat(glm::vec3(glm::vec3(firstPoint.position.x, 0, firstPoint.position.z) - distanceDown * perpendicularVector), color));
    }
}

/* The function that adds points between the given points and generates
the track (aka puts the points in the specific order of connecting and
returns the new created mesh */
Mesh* track::createTrack(
    const std::string& name,
    std::vector<VertexFormat> &aboveVertices,
    std::vector<VertexFormat> &belowVertices,
    std::vector<VertexFormat> &vertices,
    std::vector<VertexFormat> &fewerVertices)
{
    std::vector<VertexFormat> aboveMultipleVertices;
    std::vector<VertexFormat> belowMultipleVertices;

    for (int i = 0, j = 1; j < aboveVertices.size(); i++, j++) {
        VertexFormat firstPoint = aboveVertices.at(i);
        VertexFormat secondPoint = aboveVertices.at(j);

        aboveMultipleVertices.push_back(firstPoint);

        for (int k = 1; k < 30; k++) {
            float newPointRatio = (float) k / 30;
            VertexFormat newPoint = VertexFormat(firstPoint.position + newPointRatio * glm::vec3(secondPoint.position.x - firstPoint.position.x, secondPoint.position.y - firstPoint.position.y, secondPoint.position.z - firstPoint.position.z), firstPoint.color);

            aboveMultipleVertices.push_back(newPoint);
        }

        aboveMultipleVertices.push_back(secondPoint);
    }

    for (int i = 0, j = 1; j < belowVertices.size(); i++, j++) {
        VertexFormat firstPoint = belowVertices.at(i);
        VertexFormat secondPoint = belowVertices.at(j);

        belowMultipleVertices.push_back(firstPoint);

        for (int k = 1; k < 30; k++) {
            float newPointRatio = (float)k / 30;
            VertexFormat newPoint = VertexFormat(firstPoint.position + newPointRatio * glm::vec3(secondPoint.position.x - firstPoint.position.x, secondPoint.position.y - firstPoint.position.y, secondPoint.position.z - firstPoint.position.z), firstPoint.color);

            belowMultipleVertices.push_back(newPoint);
        }

        belowMultipleVertices.push_back(secondPoint);
    }

    bool impar = true;
    for (int i = 0, j = 0; i < aboveMultipleVertices.size() - 1;) {
        if (impar == true) {
            vertices.push_back(aboveMultipleVertices.at(i));
            vertices.push_back(belowMultipleVertices.at(j++));
            vertices.push_back(belowMultipleVertices.at(j));
            impar = false;
        }
        else {
            vertices.push_back(aboveMultipleVertices.at(i++));
            vertices.push_back(belowMultipleVertices.at(j));
            vertices.push_back(aboveMultipleVertices.at(i));
            impar = true;
        }
    }

    vertices.push_back(aboveMultipleVertices.at(aboveMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(belowMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(0));

    vertices.push_back(aboveMultipleVertices.at(aboveMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(0));
    vertices.push_back(aboveMultipleVertices.at(0));

    impar = true;
    for (int i = 0, j = 0; i < aboveVertices.size() - 1;) {
        if (impar == true) {
            fewerVertices.push_back(aboveVertices.at(i));
            fewerVertices.push_back(belowVertices.at(j++));
            fewerVertices.push_back(belowVertices.at(j));
            impar = false;
        }
        else {
            fewerVertices.push_back(aboveVertices.at(i++));
            fewerVertices.push_back(belowVertices.at(j));
            fewerVertices.push_back(aboveVertices.at(i));
            impar = true;
        }
    }

    fewerVertices.push_back(aboveVertices.at(aboveVertices.size() - 1));
    fewerVertices.push_back(belowVertices.at(belowVertices.size() - 1));
    fewerVertices.push_back(belowVertices.at(0));

    fewerVertices.push_back(aboveVertices.at(aboveVertices.size() - 1));
    fewerVertices.push_back(belowVertices.at(0));
    fewerVertices.push_back(aboveVertices.at(0));

    Mesh* track = new Mesh(name);
    track->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices;
    for (int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }
   
    track->InitFromData(vertices, indices);
    return track;
}

/* The function that adds points between the given points and generates
the track frame (grass) (aka puts the points in the specific order of connecting and
returns the new created mesh */
Mesh* track::createTrackFrame(
    const std::string& name,
    float length,
    float width,
    glm::vec3 color,
    int drawMode)
{
    glm::vec3 corner = glm::vec3(-length / 2, 0, -width / 2);

    std::vector<VertexFormat> initialVertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, 0, width), color),
        VertexFormat(corner + glm::vec3(length, 0, width), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color)
    };

    std::vector<VertexFormat> aboveMultipleVertices;
    std::vector<VertexFormat> belowMultipleVertices;

    VertexFormat firstPoint = initialVertices.at(0);
    VertexFormat secondPoint = initialVertices.at(1);

    aboveMultipleVertices.push_back(firstPoint);

    for (int k = 1; k < 10; k++) {
        float newPointRatio = (float)k / 10;
        VertexFormat newPoint = VertexFormat(firstPoint.position + newPointRatio * glm::vec3(secondPoint.position.x - firstPoint.position.x, secondPoint.position.y - firstPoint.position.y, secondPoint.position.z - firstPoint.position.z), firstPoint.color);

        aboveMultipleVertices.push_back(newPoint);
    }

    aboveMultipleVertices.push_back(secondPoint);

    firstPoint = initialVertices.at(3);
    secondPoint = initialVertices.at(2);

    belowMultipleVertices.push_back(firstPoint);

    for (int k = 1; k < 10; k++) {
        float newPointRatio = (float)k / 10;
        VertexFormat newPoint = VertexFormat(firstPoint.position + newPointRatio * glm::vec3(secondPoint.position.x - firstPoint.position.x, secondPoint.position.y - firstPoint.position.y, secondPoint.position.z - firstPoint.position.z), firstPoint.color);

        belowMultipleVertices.push_back(newPoint);
    }

    belowMultipleVertices.push_back(secondPoint);

    std::vector<VertexFormat> vertices;
    bool impar = true;
    for (int i = 0, j = 0; i < aboveMultipleVertices.size() - 1;) {
        if (impar == true) {
            vertices.push_back(aboveMultipleVertices.at(i));
            vertices.push_back(belowMultipleVertices.at(j++));
            vertices.push_back(belowMultipleVertices.at(j));
            impar = false;
        }
        else {
            vertices.push_back(aboveMultipleVertices.at(i++));
            vertices.push_back(belowMultipleVertices.at(j));
            vertices.push_back(aboveMultipleVertices.at(i));
            impar = true;
        }
    }

    vertices.push_back(aboveMultipleVertices.at(aboveMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(belowMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(0));

    vertices.push_back(aboveMultipleVertices.at(aboveMultipleVertices.size() - 1));
    vertices.push_back(belowMultipleVertices.at(0));
    vertices.push_back(aboveMultipleVertices.at(0));

    Mesh* rectangle = new Mesh(name);
    rectangle->SetDrawMode(drawMode);
    std::vector<unsigned int> indices;
    for (int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}
