#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform float time;
// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.

    vec2 uv = vec2(texture_coord.x - fract(-time / 100.0 / 0.7) * 0.7, texture_coord.y);

    out_color = texture(texture_1, uv);
    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if (out_color.a < 0.5)
    {
        discard;
    }
}
