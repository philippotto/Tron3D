import bpy
import os
import subprocess

### HOWTO: open this file in blender text editor. Edit your scene and make  sure only cubes are used (only translate and scale are supported for now)
## all cubes have to be prefixed by "Cube"

# This script will create newLevel.ive and newLevel.level (for osg and bullet) in data/levels/
# You can change the name of the level here:
levelName = "newLevel"

# make sure you got the TROEN and OSG_DIR environment variables
# make sure that you got osgconv.exe in OSG_DIR/bin (see dropbox)
osgPath = os.environ['OSG_DIR']
osgconvPath = osgPath + r"\bin\osgconv.exe"
troenPath = os.environ['TROEN']
levelPath = troenPath + "data\levels\\"

MODEL_PATH = levelPath + levelName + ".level"
OBJ_PATH = levelPath + levelName + ".obj"
IVE_PATH = levelPath + levelName + ".ive"


#scale blender units by
SCALE = 10.0

class LevelExporter():
	def __init__(self):
		# obstacle has to be named Cube_XXX to be registered as an obstacle
		self.obstacles = []
		for object in bpy.context.selected_objects:
			if object.name.startswith("Cube"):
				self.obstacles.append(object)
				bpy.context.scene.objects.active = object
				#bpy.ops.mesh.uv_texture_add()
				if len(object.data.uv_textures) == 0:
					bpy.ops.mesh.uv_texture_add()
				object.scale.z = abs(object.scale.z)
				object.scale.y = abs(object.scale.y)
				object.scale.x = abs(object.scale.x)

		#with open(VIEW_PATH,"w") as output_file:
		#	output_file.write(self.levelView_template().format(auto_gen_code=self.get_view_autogen()))

		with open(MODEL_PATH,"w") as output_file:
			output_file.write(self.levelModel_template().format(auto_gen_code=self.get_model_autogen()))

		bpy.ops.export_scene.obj(filepath=OBJ_PATH, check_existing=False, filter_glob="*.obj;*.mtl", use_selection=True,
		 use_animation=False, use_mesh_modifiers=True, use_edges=True,
		  use_smooth_groups=False, use_normals=True, use_uvs=True,
		   use_materials=True, use_triangles=True, use_nurbs=False,
		   use_vertex_groups=False, use_blen_objects=True, group_by_object=False,
		    group_by_material=False, keep_vertex_order=False, axis_forward='-Z',
		     axis_up='Y', global_scale=SCALE, path_mode='AUTO')

		subprocess.call([osgconvPath, OBJ_PATH, IVE_PATH])

	def get_model_autogen(self):
		#write out the cubes location and dimensions
		auto_gen_code  = ""
		for ob_index in range(len(self.obstacles)):
			obstacle = self.obstacles[ob_index]
			auto_gen_code += self.create_box_collision_shape_str().format(
														  pos_x=str(obstacle.location.x*SCALE),
														  pos_y=str(obstacle.location.y*SCALE),
														  pos_z=str(obstacle.location.z*SCALE),
														  length_x=str(obstacle.dimensions.x*SCALE),
														  length_y=str(obstacle.dimensions.y*SCALE),
														  length_z=str(obstacle.dimensions.z*SCALE),
														  quat_x=str(obstacle.rotation_quaternion.x),
														  quat_y=str(obstacle.rotation_quaternion.y),
														  quat_z=str(obstacle.rotation_quaternion.z),
														  quat_w=str(obstacle.rotation_quaternion.w),
                                                          name=str(obstacle.name),
                                                          collisionType=str(obstacle["CollisionType"]))
			if ob_index < len(self.obstacles) -1:
				auto_gen_code += "\n"
		return auto_gen_code

	def create_box_collision_shape_str(self):
		return """{pos_x}
{pos_y}
{pos_z}
{length_x}
{length_y}
{length_z}
{quat_x}
{quat_y}
{quat_z}
{quat_w}
{name}
{collisionType}"""


	def levelModel_template(self):
		return "{auto_gen_code}"

if __name__ == '__main__':
	LevelExporter()