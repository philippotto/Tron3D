import bpy

### HOWTO: open this file in blender text editor. Edit your scene and make  sure only cubes are used (only translate and scale are supported for now)
## all cubes have to be prefixed by "Cube"

#please modify to your own path, for now
VIEW_PATH = r"D:\Dropbox\Uebungen\GameProgramming\Tron\GP2013\source\view\auto_levelview.cpp"
MODEL_PATH = r"D:\Dropbox\Uebungen\GameProgramming\Tron\GP2013\source\model\auto_levelmodel.cpp"
#scale blender units by
SCALE = 10.0

class LevelExporter():
	def __init__(self):
		# obstacle has to be named Cube_XXX to be registered as an obstacle
		self.obstacles = []
		for object in bpy.data.scenes['Scene'].objects:
			if object.name.startswith("Cube"):
				self.obstacles.append(object)
	
		with open(VIEW_PATH,"w") as output_file:
			output_file.write(self.levelView_template().format(auto_gen_code=self.get_view_autogen()))

		with open(MODEL_PATH,"w") as output_file:
			output_file.write(self.levelModel_template().format(auto_gen_code=self.get_model_autogen()))


	def get_view_autogen(self):
		#write out the cubes location and dimensions
		auto_gen_code  = ""
		for ob_index in range(len(self.obstacles)):
			obstacle = self.obstacles[ob_index]
			auto_gen_code += self.create_box_stub_str().format(ob_index=ob_index,
														  pos_x=str(obstacle.location.x*SCALE),
														  pos_y=str(obstacle.location.y*SCALE),
														  pos_z=str(obstacle.location.z*SCALE), 
														  length_x=str(obstacle.dimensions.x*SCALE),
														  length_y=str(obstacle.dimensions.y*SCALE),
														  length_z=str(obstacle.dimensions.z*SCALE),
														  quat_x=str(obstacle.rotation_quaternion.x),
														  quat_y=str(obstacle.rotation_quaternion.y),
														  quat_z=str(obstacle.rotation_quaternion.z),
														  quat_w=str(obstacle.rotation_quaternion.w) )
		return auto_gen_code


	def get_model_autogen(self):
		#write out the cubes location and dimensions
		auto_gen_code  = ""
		for ob_index in range(len(self.obstacles)):
			obstacle = self.obstacles[ob_index]
			auto_gen_code += self.create_box_collision_shape_str().format(ob_index=ob_index,
														  pos_x=str(obstacle.location.x*SCALE),
														  pos_y=str(obstacle.location.y*SCALE),
														  pos_z=str(obstacle.location.z*SCALE),
														  half_length_x=str(obstacle.dimensions.x*SCALE/2.0),
														  half_length_y=str(obstacle.dimensions.y*SCALE/2.0),
														  half_length_z=str(obstacle.dimensions.z*SCALE/2.0),
														  quat_x=str(obstacle.rotation_quaternion.x),
														  quat_y=str(obstacle.rotation_quaternion.y),
														  quat_z=str(obstacle.rotation_quaternion.z),
														  quat_w=str(obstacle.rotation_quaternion.w) )
		return auto_gen_code


	def create_box_stub_str(self):
		#boilerplate code to create a box
		return """
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3({pos_x},{pos_y},{pos_z}),
								 osg::Vec3({length_x}, {length_y}, {length_z}), osg::Quat({quat_x},{quat_y},{quat_z},{quat_w}))); """

	
	def levelView_template(self):
		return """
		#include "levelview.h"
		// STD
		#include <math.h>
		// OSG
		#include <osg/ImageStream>
		#include <osg/Material>
		#include <osg/MatrixTransform>
		#include <osg/Texture2D>
		#include <osg/TexMat>
		#include <osg/Group>
		#include <osg/Geode>
		#include <osg/ShapeDrawable>
		#include <osgDB/ReadFile>
		#include <osgDB/WriteFile>
		#include <osg/Texture2D>
		#include <osg/TexEnv>
		#include <osg/TexGen>
		#include <osg/TexGenNode>


		#include "../model/levelmodel.h"
		#include "shaders.h"

		using namespace troen;

		//!!!!!!!!!!!!! WARNING: AUTO_GENERATED !!!!!!!!!!!!!!!!!!!!!!
		// If you want to change something generally, please edit obstacle_export.py, otherwise be sure to mark changes to this code otherwise it might be overwritten

		osg::ref_ptr<osg::Group> LevelView::autoConstructObstacles()
		{{
			int levelSize = m_model->getLevelSize();
			osg::ref_ptr<osg::Group> obstacleGroup = new osg::Group();

			//obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-20, -30, 10), osg::Vec3(5, 5, 20), osg::Quat(0.0, 0.0, 0.0, 1.0)));
			/////!!!! AUTO_GENERATED from here!!! /////
			{auto_gen_code}

			////// ------ to here ---------- /////

			osg::StateSet *obstaclesStateSet = obstacleGroup->getOrCreateStateSet();
			obstaclesStateSet->ref();

			obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

			setTexture(obstaclesStateSet, "data/textures/troen_box_tex.tga", 0);

			osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
			obstaclesStateSet->addUniform(textureMapU);

			osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
			obstaclesStateSet->addUniform(modelIDU);

			return obstacleGroup;
		}}
		"""
	def create_box_collision_shape_str(self):
		return """
		std::shared_ptr<btBoxShape> obstacleShape{ob_index} = std::make_shared<btBoxShape>(btVector3({half_length_x}, {half_length_y}, {half_length_z}));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState{ob_index} = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion({quat_x},{quat_y},{quat_z},{quat_w}), btVector3({pos_x}, {pos_y}, {pos_z})));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI{ob_index}(btScalar(0), obstacleMotionState{ob_index}.get(), obstacleShape{ob_index}.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody{ob_index} = std::make_shared<btRigidBody>(obstacleRigidBodyCI{ob_index});
		obstacleRigidBody{ob_index}->setUserPointer((void *)levelController);
		obstacleRigidBody{ob_index}->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape{ob_index});
		m_motionStates.push_back(obstacleMotionState{ob_index});
		m_rigidBodies.push_back(obstacleRigidBody{ob_index});

		"""

	def levelModel_template(self):
		return """
		#include "levelmodel.h"
		//bullet
		#include <btBulletDynamicsCommon.h>
		#include "LinearMath/btHashMap.h"

		using namespace troen;

		//!!!!!!!!!!!!! WARNING: AUTO_GENERATED !!!!!!!!!!!!!!!!!!!!!!
		// If you want to change something generally, please edit obstacle_export.py, otherwise be sure to mark changes to this code otherwise it might be overwritten


		void LevelModel::auto_addObstacles(const LevelController* levelController)
	{{
		// obstacles
		// TODO grab the value from origin
		btScalar levelSize = btScalar(getLevelSize());

		{auto_gen_code}

	}}
	"""
if __name__ == '__main__':
	LevelExporter()