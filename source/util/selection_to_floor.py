import bpy

#set boxes to floor
active = bpy.context.selected_objects
for object in active:
    if object.name.startswith("Cube"):
        #object.location.z = -object.dimensions.z/2
        bpy.context.scene.objects.active = object
        #bpy.ops.mesh.uv_texture_add()
        if len(object.data.uv_textures) == 0:
            bpy.ops.mesh.uv_texture_add()   
            
        object.scale.z = abs(object.scale.z)
        object.scale.y = abs(object.scale.y)
        object.scale.x = abs(object.scale.x)