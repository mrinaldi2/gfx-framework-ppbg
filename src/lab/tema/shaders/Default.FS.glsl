#version 430

uniform vec3 fill_color;
layout(location = 0) out vec4 out_color;


void main() {

	vec4 green = vec4(fill_color, 1.0);
	out_color = green;

}