#version 400

#include <pbr/deffered/geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

uniform vec3 basecolor;
uniform float metalness;
uniform float roughness;

void main()
{
    surface s;
    s.diffuse = basecolor;
    s.metalness = metalness;
    s.roughness = roughness;
    s.normal = normalize(in_vertex.normal);
    write_geometry_buffer(s);
}
