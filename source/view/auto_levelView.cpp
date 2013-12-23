
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
			
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-15.566339492797852,-85.02395629882812,0.0),
								 osg::Vec3(20.000009536743164, 20.000009536743164, 20.0), osg::Quat(0.0,0.4085420072078705,0.0,0.9127395153045654))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(72.07053184509277,0.0,0.0),
								 osg::Vec3(20.000011920928955, 20.000009536743164, 20.0), osg::Quat(-0.24187618494033813,-0.5839407444000244,0.29655107855796814,0.715937614440918))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-86.83469772338867,0.0,0.0),
								 osg::Vec3(20.000009536743164, 20.000009536743164, 20.0), osg::Quat(0.0,0.0,0.3826834559440613,0.9238795042037964))); 
			obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-14.399127960205078,98.87603759765625,0.0),
								 osg::Vec3(20.000009536743164, 20.000009536743164, 20.0), osg::Quat(0.0,0.0,0.2964495122432709,0.9550485610961914))); 

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
		