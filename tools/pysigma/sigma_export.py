#!/usr/bin/python3.5
import bpy
import json
import sys
import os
import math
import sigmacore
import logging
import static_mesh


def library_path(library):
    file_source = library.filepath if library else bpy.data.filepath
    if file_source.startswith('//'):
        file_source = os.path.join(os.path.dirname(bpy.data.filepath), file_source[2:])
    return os.path.splitext(os.path.relpath(file_source, os.getcwd()))[0]


def add_transfrom(scene_obj, blend_obj):
    scene_obj['sigma::transfrom'] = {
        'position': [blend_obj.location.x,
                     blend_obj.location.z,
                     blend_obj.location.y],

        'rotation': [math.degrees(blend_obj.rotation_euler.x),
                     math.degrees(blend_obj.rotation_euler.z),
                     math.degrees(blend_obj.rotation_euler.y)],
        'scale': [
            blend_obj.scale.x,
            blend_obj.scale.z,
            blend_obj.scale.y
        ]
    }


def add_static_mesh(scene_obj, mesh_id):
    scene_obj['sigma::graphics::static_mesh'] = mesh_id


def add_point_light(scene_obj, blend_point_light):
    scene_obj['sigma::graphics::point_light'] = {
        'color': [blend_point_light.color.r,
                  blend_point_light.color.g,
                  blend_point_light.color.b]
    }


def add_directional_light(scene_obj, blend_directional_light):
    scene_obj['sigma::graphics::directional_light'] = {
        'color': [blend_directional_light.color.r,
                  blend_directional_light.color.g,
                  blend_directional_light.color.b]
    }


def convert_scene(data):
    scene = {}
    for blend_obj in data.objects:
        scene[blend_obj.name] = scene_obj = {}
        add_transfrom(scene_obj, blend_obj)

        if blend_obj.type == 'MESH':
            source_file = library_path(blend_obj.library)
            mesh_name = blend_obj.data.name

            if blend_obj.dupli_group:
                mesh_name = blend_obj.dupli_group.name

            add_static_mesh(scene_obj, source_file + ":" + mesh_name)

        elif blend_obj.type == 'LAMP':
            if blend_obj.data.type == 'POINT':
                add_point_light(scene_obj, blend_obj.data)

            elif blend_obj.data.type == 'SUN':
                add_directional_light(scene_obj, blend_obj.data)

            else:
                logging.warning("LAMP '%s' will not be exported because it is of type '%s' which is not supported at this time." % (
                    blend_obj.name, blend_obj.data.type))

        else:
            logging.warning("object '%s' will not be exported because it is of type '%s' which is not supported at this time." % (
                blend_obj.name, blend_obj.type))

    output_file = open(os.path.splitext(bpy.data.filepath)[0] + ".scn", 'w')
    output_file.write(json.dumps(scene, sort_keys=True, indent=4, separators=(',', ':')))


if __name__ == '__main__':
    bpy.ops.wm.open_mainfile(filepath=sys.argv[1])

    for group in bpy.data.groups:
        if group.library == None:
            mesh = static_mesh.blender_group_to_static_mesh(group)
            ind = sigmacore.identifier("static_mesh",  bpy.data.filepath, group.name)
            print(ind)
            mesh.write_to_disk(ind,
                               "/home/aaron/Desktop/sigma-engine-build/data")
