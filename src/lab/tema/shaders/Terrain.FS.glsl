#version 430

layout(location = 0) out vec4 out_color;

uniform sampler2D NoiseTexture;

in vec2 texture_coords;

void main() {

	float r = texture(NoiseTexture, texture_coords).r;
	vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
	out_color = mix(green, red ,r);

}