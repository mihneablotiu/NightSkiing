#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture;
uniform vec2 textureDeplasament;
uniform int isSlope;
uniform float time;
// Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color;
    vec2 newTextCoord = texcoord;
    if (isSlope == 1) {
        newTextCoord += textureDeplasament;
    } 

    color = texture2D(texture, newTextCoord);
    if (color.w <= 0.5) {
            discard;
    }

    out_color = color;
}