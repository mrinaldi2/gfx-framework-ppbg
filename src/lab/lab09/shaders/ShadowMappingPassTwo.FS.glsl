#version 430

// Input
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D depth_texture;
uniform vec3 eye_position;
uniform vec3 light_position;
uniform vec3 light_direction;
uniform vec3 light_color;

uniform mat4 light_space_view;
uniform mat4 light_space_projection;

uniform float light_space_far_plane;

// Output
layout(location = 0) out vec4 out_color;

// Local variables and functions
const vec3 KA = vec3 (0.25);            // ambient factor
const vec3 KD = vec3 (0.3);             // diffuse factor
const vec3 KS = vec3 (0.3);             // specular factor
const float SPECULAR_EXPONENT = 40.0;   // specular exponent

const float spot_angle = 45.0;

const float G_SCATTERING = -0.1f;

// Mie scattering approximated with Henyey-Greenstein phase function
float ComputeScattering(float cosAngle)
{
    float result = 1.0f - G_SCATTERING * G_SCATTERING;
    result /= (4.0f * 3.14 * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * cosAngle, 1.5f));
    return result;
}

vec3 PhongLight()
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);

    float dist = distance(light_position, world_position);

    if (dist > light_space_far_plane)
        return vec3(0);

    float att = pow((light_space_far_plane - dist) /
        light_space_far_plane, 2);

    float dot_specular = dot(N, L);
    vec3 specular = vec3(0);
    if (dot_specular > 0)
    {
        vec3 V = normalize(eye_position - world_position);
        vec3 H = normalize(L + V);
        specular = KS * pow(max(dot(N, H), 0), SPECULAR_EXPONENT);
    }

    vec3 diffuse = KD * max(dot_specular, 0);

    return att * (diffuse + specular);
}

bool IsIlluminated(vec3 point_position, float bias)
{
    //transform as camera is at light position using view and perspecive matrixes
    vec4 light_space_pos = light_space_projection * light_space_view * vec4 (point_position, 1.0f);

    //perspective division
    light_space_pos = light_space_pos / light_space_pos.w;

    //remappere pentru spatiu depth
    float light_space_depth = light_space_pos.z * 0.5f + 0.5f;

    //remappare in spatiu texture
    vec2 depth_map_pos = light_space_pos.xy * 0.5f + 0.5f;

    //control daca se afla in textura
	bvec2 a = greaterThan(depth_map_pos, vec2(1.0, 1.0));
	bvec2 b = lessThan(depth_map_pos, vec2(0.0, 0.0));

	if (any(bvec2(any(a), any(b)))) {
		return false;
	}

    float depth = texture(depth_texture, depth_map_pos).x;

    return light_space_depth - bias < depth;
}

float ShadowFactor()
{
    return IsIlluminated(world_position, 0.0001f) ? 1.0f : 0.0f;
}

vec3 SpotLight()
{
    float light_intensity = 10.0f;

    vec3 L = normalize(light_position - world_position);

    float cut_off = radians(spot_angle);
    float spot_light = dot(-L, light_direction);

    if (spot_light > cos(cut_off)) {

        float spot_light_limit = cos(cut_off);
 
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        // TODO(student): Multiply the shadow factor with the
        // result of the lighting calculation. Don't apply
        // this factor to the ambient component as well.
        return light_att_factor * light_intensity * PhongLight() * ShadowFactor() + KA;
    }

    return KA;
}

float VolumetricIllumination()
{
    vec3 ray_direction = world_position - eye_position;

    float illuminated_samples_count = 0;
    int sample_count = 0;

    vec3 point_position = eye_position;
    // TODO(student): Sample several hundreds of points between the
    // observer's coordinate and the world-space coordinate of the
    // point rendered in the current fragment. Quantify the number of
    // sampled points that are illuminated by the spot light source,
    // IsIlluminated() and the number of sampled points in total.
    for (int i = 0; i < 100; i++)
	{
		point_position += ray_direction * 0.01f;
		sample_count++;
        float scattering = ComputeScattering(dot(normalize(ray_direction), normalize(point_position - light_position)));
		if (IsIlluminated(point_position, 0.0f))
		{
            illuminated_samples_count += scattering * 0.01f;
		}
	}

    return illuminated_samples_count * 5;
}

void main()
{
    vec4 texture_color = texture(texture_1, texture_coord);

    if (texture_color.a < 0.5)
    {
        discard;
    }

    vec3 color = vec3(texture_color) * SpotLight() + VolumetricIllumination();

    out_color = vec4(color, 1.0);
}
