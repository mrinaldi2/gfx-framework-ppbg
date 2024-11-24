#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    vec4 first_color = texture(texture_1, texture_coord);
    vec4 second_color = texture(texture_2, texture_coord);
    second_color = vec4(second_color.rrr, 1.0);
    if (second_color.r < 0.7) 
    {
	    out_color = mix(texture(texture_3, texture_coord), first_color, smoothstep(0.15,0.30, second_color.r));
    } else {
		out_color = mix(first_color, texture(texture_4, texture_coord), smoothstep(0.70,0.80, second_color.r));
	}

    if (out_color.a < 0.5)
    {
        //discard;
    }
}
