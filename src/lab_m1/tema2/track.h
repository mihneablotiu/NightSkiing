#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace track
{
    void generatePoints(glm::vec3 color, std::vector<VertexFormat> initialVertices, std::vector<VertexFormat>& aboveVertices, std::vector<VertexFormat>& belowVertices, float distanceUpAndDown);
    Mesh* createTrack(const std::string& name, std::vector<VertexFormat>& aboveVertices, std::vector<VertexFormat>& belowVertices, std::vector<VertexFormat>& vertices, std::vector<VertexFormat>& fewerVertices);
    Mesh* createTrackFrame(const std::string& name, float length, float width, glm::vec3 color, int drawMode);
}
