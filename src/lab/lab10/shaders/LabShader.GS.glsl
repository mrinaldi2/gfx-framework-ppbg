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
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec2 texture_coord;

void main()
{
	// TODO(student): Emit the vertices of the triangle for which
	// information is received in the input attributes. Set the
	// following information for each vertex:
	//   - The coordinate of the vertex in clip space, transformed
	//     from world space, as received from the vertex shader
	//   - Texture coordinate received from the vertex shader.
	gl_Position = Projection * View * gl_in[0].gl_Position;
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * gl_in[1].gl_Position;
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * gl_in[2].gl_Position;
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

	gl_Position = Projection * View * (gl_in[0].gl_Position + vec4(0.0, 0.0, -1.0, 0.0));
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[1].gl_Position + vec4(0.0, 0.0, -1.0, 0.0));
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[2].gl_Position + vec4(0.0, 0.0, -1.0, 0.0));
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

	gl_Position = Projection * View * (gl_in[0].gl_Position + vec4(0.0, 0.0, -2.0, 0.0));
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[1].gl_Position + vec4(0.0, 0.0, -2.0, 0.0));
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[2].gl_Position + vec4(0.0, 0.0, -2.0, 0.0));
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

	gl_Position = Projection * View * (gl_in[0].gl_Position + vec4(0.0, 0.0, -3.0, 0.0));
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[1].gl_Position + vec4(0.0, 0.0, -3.0, 0.0));
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[2].gl_Position + vec4(0.0, 0.0, -3.0, 0.0));
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

	gl_Position = Projection * View * (gl_in[0].gl_Position + vec4(-2.0, 0.0, 0.0, 0.0));
	texture_coord = v_texture_coord[0];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[1].gl_Position + vec4(-2.0, 0.0, 0.0, 0.0));
	texture_coord = v_texture_coord[1];
	EmitVertex();

	gl_Position = Projection * View * (gl_in[2].gl_Position + vec4(-2.0, 0.0, 0.0, 0.0));
	texture_coord = v_texture_coord[2];
	EmitVertex();

	EndPrimitive();

}
