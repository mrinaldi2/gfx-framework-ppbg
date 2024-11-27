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

float getScalar() {
	return Scale * ExplosionTime;
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

	

	gl_Position = Projection * View * gl_in[0].gl_Position + vec4(normal * getScalar(), 0);
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * gl_in[1].gl_Position + vec4(normal * getScalar(), 0);
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * gl_in[2].gl_Position + vec4(normal * getScalar(), 0);
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

}
