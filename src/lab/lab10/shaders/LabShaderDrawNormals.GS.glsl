#version 430

// Input and output topologies
layout(triangles) in;
layout(line_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];

// Uniform properties
uniform mat4 View;
uniform mat4 Model;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec3 color;

void main()
{
	// TODO(student): Emit the vertices of the triangle for which
	// information is received in the input attributes. Set the
	// following information for each vertex:
	//   - The coordinate of the vertex in clip space, transformed
	//     from world space, as received from the vertex shader
	//   - Texture coordinate received from the vertex shader.
	
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;
 
	vec3 center = (p1 + p2 + p3) / 3;
 
	vec3 v12 = normalize(p2 - p1);
	vec3 v13 = normalize(p3 - p1);
 
	// produs vectorial
	vec3 normal = cross(v12, v13);

	gl_Position = Projection * View * vec4(center, 1.0);
	color = vec3(0,0,1);
	EmitVertex();

	gl_Position = Projection * View * vec4(center + normal * 0.05, 1.0);
	color = vec3(0,0,1);
	EmitVertex();

	EndPrimitive();

}
