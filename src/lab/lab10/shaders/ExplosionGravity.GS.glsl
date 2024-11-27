#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];

// Uniform properties
uniform mat4 View;
uniform mat4 Model;
uniform mat4 Projection;
uniform float Scale;
uniform float ExplosionTime;
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec2 texture_coord;

vec3 getDirection(vec3 normal) {
	return normal * ExplosionTime;
}

vec3 getGravity() {
	return 0.5f * vec3(0, -1, 0) * pow(ExplosionTime,2);
}

vec4 getScaledDirection(vec3 normal) {
	vec3 final_position = getDirection(normal) + getGravity();
	return vec4(final_position.x, final_position.y, final_position.z, 0);
}

void main()
{
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;
 
	vec3 v12 = normalize(p2 - p1);
	vec3 v13 = normalize(p3 - p1);
 
	// produs vectorial
	vec3 normal = cross(v12, v13);

	
	vec4 final_pos =  (gl_in[0].gl_Position + getScaledDirection(normal));
	gl_Position = Projection * View * vec4(final_pos.x, max(final_pos.y, 0), final_pos.z, 1.0);
	texture_coord = v_texture_coord[0];
	EmitVertex();

	final_pos = (gl_in[1].gl_Position + getScaledDirection(normal));
	gl_Position = Projection * View * vec4(final_pos.x, max(final_pos.y, 0), final_pos.z, 1.0);
	texture_coord = v_texture_coord[1];
	EmitVertex();

	final_pos = (gl_in[2].gl_Position + getScaledDirection(normal));
	gl_Position = Projection * View * vec4(final_pos.x, max(final_pos.y, 0), final_pos.z, 1.0);
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

}
