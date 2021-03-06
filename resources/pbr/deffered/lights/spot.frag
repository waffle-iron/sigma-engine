#version 400

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
#include <shadow_sampling.glsl>
// clang-format on

in spot_light
{
    vec3 color;
    float intensity;
    vec3 position;
    vec3 direction;
    float cutoff;
}
in_light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position - s.position);
    vec3 L = in_light.position - s.position;
    float light_distance = length(L);
    L = normalize(L);

    // TODO Use eq (9) from
    // http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
    // float att = smoothstep(1, 0, light_distance / in_light.radius) / (light_distance * light_distance);
    float att = 1.0 / (light_distance * light_distance);

    float factor = dot(L, in_light.direction);

    if (factor > in_light.cutoff) {
        att *= (1.0 - (1.0 - factor) * 1.0 / (1.0 - in_light.cutoff));
    } else {
        att = 0;
    }

    vec4 light_space_position = light_projection_view_matrix * vec4(s.position, 1);

    float depth = length(s.position - in_light.position);
    float shadow = calculate_shadow(in_shadow_map, light_space_position, depth); //bias=.00002

    out_image = shadow * att * in_light.intensity * in_light.color * compute_lighting(s, L, V);
}
