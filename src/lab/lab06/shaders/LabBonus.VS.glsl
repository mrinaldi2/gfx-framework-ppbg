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

// Function to create pseudo-random values based on position
float random(vec3 seed) {
    return fract(sin(dot(seed, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

float improvedRandom(vec3 position) {
    float rand1 = random(position);
    float rand2 = random(position.yzx + vec3(23.1406, 39.367, 73.291));
    float rand3 = random(position.zxy + vec3(17.426, 55.731, 92.413));
    return fract((rand1 + rand2 + rand3) * 0.333);
}

float getSinBasedValue(vec3 f_position, float time, float value) {
    float rand = improvedRandom(f_position);
    return rand * sin(time*5*rand) * (value/2);
}

void main()
{
    // TODO(student): Send output to fragment shader
    color = v_position;
    vec3 f_position = v_position + v_normal * 0.2;
    vec3 worldNormal = normalize(f_position);
    float scale = mix(0.0, 0.8, improvedRandom(f_position));

    vec3 scaledPosition = f_position + worldNormal * getSinBasedValue(f_position, Time, scale);
    gl_Position = Projection * View * Model * vec4(scaledPosition, 1.0);
}