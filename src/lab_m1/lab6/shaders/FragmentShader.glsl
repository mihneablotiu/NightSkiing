#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 fragmentsColor;
in vec3 normalColor;
in float shaderTime;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
       out_color = vec4(sin(shaderTime), cos(shaderTime), cos(shaderTime), 1);

}
