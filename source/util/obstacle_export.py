import bpy
### HOWTO: open this file in blender text editor. Edit your scene and make  sure only cubes are used (only translate and scale are supported for now)
## all cubes have to be prefixed by "Cube"


#please modify to your own path, for now
FILE_PATH = r"D:\Dropbox\Uebungen\GameProgramming\Tron\GP2013\source\view\auto_levelView.cpp"






Cpp_string_levelView = """
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

osg::ref_ptr<osg::Geode> LevelView::autoConstructObstacles()
{{
	int levelSize = m_model->getLevelSize();
	osg::ref_ptr<osg::Geode> obstacleGeode = new osg::Geode();

	{auto_gen_code}
	
	osg::StateSet *obstaclesStateSet = obstacleGeode->getOrCreateStateSet();
	obstaclesStateSet->ref();
	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

	setTexture(obstaclesStateSet, "data/textures/troen_box_tex.tga", 0);

	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	obstaclesStateSet->addUniform(modelIDU);

	return obstacleGeode;
}}
"""

#for reference:
# // 

output_file = open(FILE_PATH,"w")

#boilerplate code to create a box
create_box_str = """
osg::ref_ptr<osg::Box> obstacle{ob_index}
	= new osg::Box({vec3_location}, {length_x}, {length_y}, {length_z});
osg::ref_ptr<osg::ShapeDrawable> boxDrawable{ob_index}
	= new osg::ShapeDrawable(obstacle{ob_index}); """


# obstacle has to be named Cube_XXX to be registered as an obstacle
obstacles = []
for object in bpy.data.scenes['Scene'].objects:
	if object.name.startswith("Cube"):
		obstacles.append(object)

#write out the cubes location and dimenions
auto_gen_code  = ""
for ob_index in range(len(obstacles)):
	obstacle = obstacles[ob_index]
	position_str = "osg::Vec3(" + str(obstacle.location.x) + "," + \
                    str(obstacle.location.y) + "," + str(obstacle.location.z) + ")"
	auto_gen_code += create_box_str.format(ob_index=ob_index,
												  vec3_location=position_str, 
												  length_x=str(obstacle.dimensions.x),
												  length_y=str(obstacle.dimensions.y),
												  length_z=str(obstacle.dimensions.z) )
	auto_gen_code += "\nobstacleGeode->addDrawable(boxDrawable{ob_index});\n".format(ob_index=str(ob_index))



output_file.write(Cpp_string_levelView.format(auto_gen_code=auto_gen_code))
output_file.close()

