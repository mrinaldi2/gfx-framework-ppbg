#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D texture_2;

// Output
out vec2 texture_coord;


void main()
{
    float height = texture(texture_2, v_texture_coord).r * 6.0f;
    //height = v_position.y + height;
    // TODO(student): Pass v_texture_coord as output to fragment shader
	texture_coord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(v_position.x, height, v_position.z, 1.0);
}
