import sigmacore
import bpy
import logging
import bmesh
import itertools


def grouper(n, iterable, fillvalue=None):
    "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return itertools.zip_longest(*args, fillvalue=fillvalue)


def triangulate_mesh(mesh):
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()


def blender_object_to_static_mesh(obj, apply_transfrom):
    try:
        # TODO look into calc_tessface and calc_undeformed
        mesh = obj.to_mesh(bpy.context.scene, True, 'PREVIEW',
                           calc_tessface=False, calc_undeformed=True)
    except:
        return None

    if apply_transfrom:
        mesh.transform(obj.matrix_world)

    triangulate_mesh(mesh)

    mesh.calc_tangents()

    uv_layer = mesh.uv_layers.active.data

    mesh.calc_normals_split()
    loops = mesh.loops

    all_verts = [None] * 3 * len(mesh.polygons)
    for i, poly in enumerate(mesh.polygons):
        for j, l_idx in enumerate(poly.loop_indices):
            all_verts[3 * i + j] = [mesh.vertices[mesh.loops[l_idx].vertex_index].co[:],
                                    loops[l_idx].normal[:],
                                    loops[l_idx].tangent[:],
                                    uv_layer[l_idx].uv]

    final_verts = []
    final_ind = []  # 3*len(polygons)
    for vert in all_verts:
        try:
            i = final_verts.index(vert)
        except ValueError:
            i = len(final_verts)
            final_verts.append(vert)
        final_ind.append(i)

    final_ind = list(grouper(3, final_ind))

    sm = sigmacore.static_mesh_data()
    for i, v in enumerate(final_verts):
        vert = sigmacore.static_mesh_data.vertex()

        vert.position.x = v[0][0]
        vert.position.y = v[0][2]
        vert.position.z = -v[0][1]

        vert.normal.x = v[1][0]
        vert.normal.y = v[1][2]
        vert.normal.z = -v[1][1]

        vert.tangent.x = v[2][0]
        vert.tangent.y = v[2][2]
        vert.tangent.z = -v[2][1]

        vert.texcoord.x = v[3][0]
        vert.texcoord.y = v[3][1]

        sm.vertices.append(vert)

    x = 0
    for poly in final_ind:
        tri = sigmacore.static_mesh_data.triangle()
        tri[:] = poly
        sm.triangles.append(tri)

    # TODO real material
    sm.material = sigmacore.identifier("material", "proprietary/rustediron2")
    return sm


def blender_group_to_static_mesh(group):
    static_mesh = None
    for group_obj in group.objects:
        converted_mesh = blender_object_to_static_mesh(group_obj, True)
        if static_mesh == None:
            static_mesh = converted_mesh
        elif converted_mesh != None:
            static_mesh.merge(converted_mesh)
    return static_mesh
