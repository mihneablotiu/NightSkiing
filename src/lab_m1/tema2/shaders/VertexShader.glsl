#version 330

// Input
// Get vertex attributes from each location
layout(location = 0) in vec3 vertexPositions;
layout(location = 1) in vec3 normalPositions;
layout(location = 2) in vec3 textureCoordinates;
layout(location = 3) in vec3 colors;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 carPosition;

// Output
// Output values to fragment shader
out vec3 fragmentsColor;

void main()
{
    // Send output to fragment shader
    fragmentsColor = colors;

    // Compute gl_Position
    float scaleFactor = 0.04;

    vec4 vertexPositionWorld = Model * vec4(vertexPositions, 1.0);
    vec4 carPositionWorld = vec4(carPosition, 1.0);
    vec4 newVertex = vertexPositionWorld;

    // Changing the y component of all the vertices for the visual effect
    newVertex.y = newVertex.y - pow(distance(carPositionWorld, vertexPositionWorld), 2) * scaleFactor;
    gl_Position = Projection * View * newVertex;
}
