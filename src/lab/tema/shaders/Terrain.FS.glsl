#version 430

layout(location = 0) out vec4 out_color;

uniform sampler2D NoiseTexture;

uniform sampler2D SnowTexture;
uniform sampler2D GroundTexture;

in vec2 texture_coords;

void main() {

	float r = texture(NoiseTexture, texture_coords).r;
	vec4 ground = texture(GroundTexture, texture_coords);
    vec4 snow = texture(SnowTexture, texture_coords);
    if (r < 0.5)
    {
		out_color = mix(ground, snow, smoothstep(0.4, 0.5, r));
	}
	else
	{
		out_color = snow;
	}

    if (out_color.a < 0.5)
    {
        //discard;
    }

}