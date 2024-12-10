#version 430

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
uniform int no_of_generated_points;
uniform float max_translate;
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

    for (int i = 0; i < no_of_generated_points; i++) {
        // Compute the current and next Bézier curve points
        float u = float(i) / float(no_of_generated_points);
        
        float dist = max_translate / float(no_of_instances);

        vec3 p0_curr = translateX(control_p0, instance[0] * dist);
        vec3 p1_curr = translateX(control_p1, instance[0] * dist);
        vec3 p2_curr = translateX(control_p2, instance[0] * dist);
        vec3 p3_curr = translateX(control_p3, instance[0] * dist);

        vec3 p0_next = translateX(control_p0, (instance[0] + 1) * dist);
        vec3 p1_next = translateX(control_p1, (instance[0] + 1) * dist);
        vec3 p2_next = translateX(control_p2, (instance[0] + 1) * dist);
        vec3 p3_next = translateX(control_p3, (instance[0] + 1) * dist);
            
        vec3 point_curr = pow(1 - u, 3) * p0_curr + 
                            3 * pow(1 - u, 2) * u * p1_curr + 
                            3 * (1 - u) * pow(u, 2) * p2_curr + 
                            pow(u, 3) * p3_curr;

        vec3 point_next = pow(1 - u, 3) * p0_next + 
                            3 * pow(1 - u, 2) * u * p1_next + 
                            3 * (1 - u) * pow(u, 2) * p2_next + 
                            pow(u, 3) * p3_next;

        // Emit vertices for the triangle strip
        gl_Position = Projection * View * vec4(point_curr, 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(point_next, 1);
        EmitVertex();
    }
    EndPrimitive();

}
