#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 vertexPositions;
layout(location = 1) in vec3 normalPositions;
layout(location = 2) in vec3 textureCoordinates;
layout(location = 3) in vec3 colors;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 fragmentsColor;
out vec3 normalColor;
out float shaderTime;

void main()
{
    // TODO(student): Send output to fragment shader
    fragmentsColor = colors;
    normalColor = normalPositions;
    shaderTime = Time;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(vertexPositions * cos(shaderTime), 1.0);
}
