#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec3 color;

void EmitPoint(vec3 position, vec3 c)
{
    color = c;
    gl_Position = Projection * View * vec4(position, 1);
    EmitVertex();
}

vec3 compute_reflected_point(vec3 p1, vec3 p2, vec3 p3) {
    vec3 center = (p1 + p2 + p3) / 3;
    vec3 lineCenter = (p1 + p2) / 2;
    vec3 dir = normalize(lineCenter - center);
    return center + dir * 1.12;
}


void main()
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    // TODO(student): Emit the vertices of the triangle for which
    // information is received in the input attributes. Use EmitPoint()
    // and set a color information associated with each vertex.

    EmitPoint(p1, vec3(1, 1, 0));
    EmitPoint(p2, vec3(1, 1, 0));
    EmitPoint(p3, vec3(1, 1, 0));
    EndPrimitive();

    EmitPoint(p1, vec3(1, 1, 0));
    EmitPoint(p2, vec3(1, 1, 0));
    EmitPoint(compute_reflected_point(p1, p2, p3), vec3(1, 0, 0));
    EndPrimitive();

    EmitPoint(p1, vec3(1, 1, 0));
    EmitPoint(p3, vec3(1, 1, 0));
    EmitPoint(compute_reflected_point(p1, p3, p2), vec3(1, 0, 0));
    EndPrimitive();

    EmitPoint(p2, vec3(1, 1, 0));
    EmitPoint(p3, vec3(1, 1, 0));
    EmitPoint(compute_reflected_point(p2, p3, p1), vec3(1, 0, 0));
    EndPrimitive();


}
