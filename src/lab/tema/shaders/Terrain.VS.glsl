#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform float gridSize;
uniform float gridHalfSize;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 eye_position;

uniform sampler2D NoiseTexture;

float heightScale = 3;

out vec2 texture_coords;

void main() {

    float normalizedX = (v_position.x + gridHalfSize) / gridSize;
    float normalizedZ = (v_position.z + gridHalfSize) / gridSize;

    float height = texture(NoiseTexture, vec2(normalizedX, normalizedZ)).r;

    // Adjust vertex position
    vec3 modifiedPos = v_position;
    modifiedPos.y = (height * pow(height, 0.8f) * 30.0f) - 2.5f;

    vec4 world_position = Model * vec4(modifiedPos, 1.0);
    float distance = length(eye_position - world_position.xyz);
    float curvature = distance * 1.2f;
    modifiedPos.y = modifiedPos.y - curvature;

	gl_Position = Projection * View * Model * vec4(modifiedPos, 1.0);
    texture_coords = vec2(normalizedX, normalizedZ);

}