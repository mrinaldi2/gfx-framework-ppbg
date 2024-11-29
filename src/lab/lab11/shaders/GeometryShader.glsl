#version 430

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(line_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
// TODO(student): Declare any other uniforms here

// Input
in int instance[2];

vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}

vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}

void main()
{
    const int SURFACE_TYPE_TRANSLATION = 0;
    const int SURFACE_TYPE_ROTATION = 1;

    // TODO(student): Rather than emitting vertices for the control
    // points, you must emit vertices that approximate the curve itself.
    gl_Position = Projection * View * vec4(control_p0, 1);   EmitVertex();
    vec3 bt = pow(0.75f, 3) * control_p0 + 3 * pow(0.75f, 2) * 0.25f * control_p1 + 3 * 0.75f * pow(0.25f, 2) * control_p2 + pow(0.25f, 3) * control_p3;
    gl_Position = Projection * View * vec4(bt, 1);   EmitVertex();
    vec3 bt2 = pow(0.25f, 3) * control_p0 + 3 * pow(0.25f, 2) * 0.75f * control_p1 + 3 * 0.25f * pow(0.75f, 2) * control_p2 + pow(0.75f, 3) * control_p3;
    gl_Position = Projection * View * vec4(bt2, 1);   EmitVertex();
    gl_Position = Projection * View * vec4(control_p3, 1);   EmitVertex();
    EndPrimitive();
}
