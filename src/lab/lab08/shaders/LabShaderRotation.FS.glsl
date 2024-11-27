#version 330

// Input
in vec2 texture_coord;
in vec3 normal;
// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float time;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    float y = texture_coord.y - 0.5f;
    vec2 uv = vec2(texture_coord.x - time * (1 - y) * 0.2, texture_coord.y);
    out_color = vec4(normal * 0.5 + 0.5, 1.0);

    out_color = texture(texture_1, uv);
    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if (out_color.a < 0.5)
    {
        discard;
    }
}
