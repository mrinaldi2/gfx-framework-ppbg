#version 330

// Input
in vec2 texture_coord;
in vec3 normal;
// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.

    out_color = vec4(normal * 0.5 + 0.5, 1.0);

    //out_color = texture(texture_1, texture_coord);
    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if (out_color.a < 0.5)
    {
        discard;
    }
}
