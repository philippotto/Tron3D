#include "bikeview.h"

#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>

#include "../model/bikemodel.h"

using namespace troen;

BikeView::BikeView(std::shared_ptr<BikeModel> model)
{
	m_model = model;

	m_node = new osg::Group();

	osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
	//pat->addChild(osgDB::readNodeFile("data/models/cycle/HQ_Movie cycle.obj"));
	pat->addChild(osgDB::readNodeFile("data/models/cessna.osgt"));

	m_node->addChild(pat);
}