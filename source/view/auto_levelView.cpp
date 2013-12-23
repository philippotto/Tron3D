
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
		{
			int levelSize = m_model->getLevelSize();
			osg::ref_ptr<osg::Group> obstacleGroup = new osg::Group();

			//obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-20, -30, 10), osg::Vec3(5, 5, 20), osg::Quat(0.0, 0.0, 0.0, 1.0)));
			/////!!!! AUTO_GENERATED from here!!! /////
			
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(342.88257598876953,918.4856414794922,71.01015567779541),
								 osg::Vec3(1020.994873046875, 898.2110595703125, 6.920419335365295), osg::Quat(0.0,0.0,0.0,1.0))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(70.70180892944336,-5.369071960449219,-5.070757865905762),
								 osg::Vec3(20.000009536743164, 20.000009536743164, 20.0), osg::Quat(0.0,0.0,0.0,1.0))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(568.7586212158203,-879.2779541015625,66.1803674697876),
								 osg::Vec3(683.0246734619141, 321.9303894042969, 19.53296184539795), osg::Quat(0.0,0.0,0.0,1.0))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(988.9772033691406,162.55027770996094,69.9608039855957),
								 osg::Vec3(288.5529136657715, 2405.4783630371094, 11.89049243927002), osg::Quat(0.0,0.0,0.0,1.0))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-10.782440900802612,-778.228759765625,72.63442039489746),
								 osg::Vec3(531.1703491210938, 531.1703491210938, 8.442516922950745), osg::Quat(0.0,0.0,0.0,1.0))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-10.782440900802612,-264.0263366699219,30.399227142333984),
								 osg::Vec3(278.0601501464844, 516.7617797851562, 9.876872301101685), osg::Quat(-2.33497376989078e-09,0.08276040852069855,0.9965696334838867,-4.364898131825612e-08))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-80.16692161560059,-5.369071960449219,-5.070757865905762),
								 osg::Vec3(20.000009536743164, 20.000009536743164, 20.0), osg::Quat(0.0,0.0,0.0,1.0))); 

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
		}
		