import bpy

VALUE = 0


active = bpy.context.selected_objects
for object in active:
    if object.name.startswith("Cube"):
        object["CollisionType"] = VALUE               # create and assign an ID-property
        object["_RNA_UI"] = {"CollisionType": {"min":0, "max":1,"description":"0 for no health substraction on collision"}}
        print(object["CollisionType"])
        
        


