#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 color;

void main()
{
    // TODO(student): Send output to fragment shader
    color = v_color;

    vec3 f_position = v_position * abs(sin(Time * 0.7)) * 0.6 + v_position * 0.8f;
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(f_position, 1.0);
}