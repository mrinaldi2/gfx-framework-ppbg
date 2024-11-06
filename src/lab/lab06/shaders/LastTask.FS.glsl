#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 color;

// Uniform properties
uniform float Time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color, 1.0);

}
