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

float getSinBasedValue(float time) {
    return (sin(time) + 1.0) * 1.5;
}

void main()
{
    // TODO(student): Send output to fragment shader
    color = v_color;
    // TODO(student): Compute gl_Position
    vec4 position = View * Model * vec4(v_position, 1.0);
    gl_Position = Projection * (position + vec4(0, getSinBasedValue(Time), 0, 0));
}