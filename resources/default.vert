#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

void main()
{
    out_vertex.position = model_matrix * vec4(in_position, 1);
    out_vertex.normal = normalize(normal_matrix * in_normal);
    out_vertex.tangent = normalize(normal_matrix * in_tangent);
    out_vertex.bitangent = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;
    gl_Position = projection_view_matrix * out_vertex.position;
}
