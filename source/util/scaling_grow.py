import bpy
import math

# The following function is adapted from 
# Nick Keeline "Cloud Generator" addNewObject 
# from object_cloud_gen.py (an addon that comes with the Blender 2.6 package)
#
SCALE = 0.95
NUMBER = 10


def duplicateObject(name, copyobj):
  # Create new mesh
  mesh = bpy.data.meshes.new(name)

  # Create new object associated with the mesh
  ob_new = bpy.data.objects.new(name, mesh)

  # Copy data block from the old object into the new object
  ob_new.data = copyobj.data.copy()
  ob_new.scale = copyobj.scale
  
  ob_new.location = copyobj.location

  # Link new object to the given scene and select it
  bpy.context.scene.objects.link(ob_new)
  ob_new.select = True

  return ob_new


active = bpy.context.scene.objects.active

for i in range(NUMBER):
  split = active.name.split(".")
  if len(split) == 1:
    split.append(1)
  new_name = split[0] + "." + str(int(split[1]) + 1)
  count = int(split[1])
  while bpy.data.objects.find(new_name) != -1:
    count += 1
    new_name =split[0] + "." + str(count)

  newobj = duplicateObject(new_name,active)
  newobj.location.y -= active.dimensions.y
  newobj.scale.z *= SCALE
  active = newobj