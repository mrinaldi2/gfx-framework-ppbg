#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 color;

uniform float Time;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // TODO(student): Write pixel out color
    vec3 f_color = vec3(color.r, color.g, color.b);
    out_color = vec4(f_color, 1);

}
