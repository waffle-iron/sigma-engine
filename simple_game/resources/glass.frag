#version 330

#define SIGMA_ENGINE_TRANSPARENT
#include <pbr/deffered/geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

uniform sampler2D basecolor_map;
uniform sampler2D metallic_map;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

void main()
{
    vec2 uv = gl_FragCoord.xy / view_port_size;

    surface s;
    mat3 tbn = mat3(in_vertex.tangent, in_vertex.binormal, in_vertex.normal);

    // TODO do this conversion on the cpu
    vec4 base_texture = texture(basecolor_map, in_vertex.texcoord);
    vec3 surface_color = pow(base_texture.rgb, vec3(2.2));
    vec3 background_color = texture(in_image, uv).rgb;
    s.diffuse = mix(surface_color * background_color, surface_color, base_texture.a);
    s.metalness = texture(metallic_map, in_vertex.texcoord).r;
    s.roughness = texture(roughness_map, in_vertex.texcoord).r;
    s.normal = normalize(tbn * (texture(normal_map, in_vertex.texcoord).xyz * 2.0 - 1.0));
    write_geometry_buffer(s);
}
