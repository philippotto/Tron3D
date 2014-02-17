import bpy
import math
from mathutils import Vector, Euler

RAD_DEG = 57.2957795
PI = 3.14159265



for current_obj in bpy.context.selected_objects:
    i = current_obj.data.polygons.active
    poly = current_obj.data.polygons[i]
    normal = poly.normal
    print("normal ",normal)
    
    up = Vector((0.0,0.0,1.0))
    
    angle = PI/2 - normal.angle(up)
    print("angle" + str(angle))
    
    
    #bpy.ops.object.mode_set(mode = 'EDIT')
    
    current_obj.rotation_euler.x = angle
    bpy.ops.object.transform_apply(rotation=True)
    current_obj.rotation_euler.x = -angle   