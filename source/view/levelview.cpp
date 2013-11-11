#include "levelview.h"

#include <math.h>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>

using namespace troen;


LevelView::LevelView()
{
	initialize();
}


void LevelView::initialize() {
	
	m_node = new osg::Group();

	osg::ref_ptr<osg::Geode> levelGeode = new osg::Geode();

	// ground
	osg::ref_ptr<osg::Box> ground
		= new osg::Box(osg::Vec3(0, 0, 0), 1000, 1000, 1);
	
	osg::ref_ptr<osg::ShapeDrawable> groundDrawable
		= new osg::ShapeDrawable(ground);


	// wall right
	osg::ref_ptr<osg::Box> wallRight
		= new osg::Box(osg::Vec3(500, 0, 50), 1, 1000, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableRight
		= new osg::ShapeDrawable(wallRight);

	// wall left
	osg::ref_ptr<osg::Box> wallLeft
		= new osg::Box(osg::Vec3(-500, 0, 50), 1, 1000, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableLeft
		= new osg::ShapeDrawable(wallLeft);
	
	
	// wall back
	osg::ref_ptr<osg::Box> wallBack
		= new osg::Box(osg::Vec3(0, -500, 50), 1000, 1, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableBack
		= new osg::ShapeDrawable(wallBack);

	// wall front
	osg::ref_ptr<osg::Box> wallFront
		= new osg::Box(osg::Vec3(0, 500, 50), 1000, 1, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableFront
		= new osg::ShapeDrawable(wallFront);


	// fence
	osg::Box *fence = new osg::Box(osg::Vec3(0, 0, 100), 1, 10, 10);
	osg::ShapeDrawable* fenceDrawable = new osg::ShapeDrawable(fence);
	levelGeode->addDrawable(fenceDrawable);


	levelGeode->addDrawable(groundDrawable);
	levelGeode->addDrawable(wallDrawableLeft);
	levelGeode->addDrawable(wallDrawableRight);
	levelGeode->addDrawable(wallDrawableFront);
	levelGeode->addDrawable(wallDrawableBack);

	m_node->addChild(levelGeode);
}

osg::ref_ptr<osg::Group> LevelView::getNode() {
	return m_node;
}