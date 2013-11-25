#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>

using namespace troen;

FenceView::FenceView()
{
	
}

void FenceView::updateFence()
{
	// fence
	m_node = new osg::Group();

	osg::Box *fence = new osg::Box(osg::Vec3(0, 0, 100), 1, 10, 10);
	osg::ShapeDrawable* fenceDrawable = new osg::ShapeDrawable(fence);
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(fenceDrawable);

	m_node->addChild(geode);
}