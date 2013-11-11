#include <osg/Vec3>

#include <osg/Node>
#include <osg/DisplaySettings>
#include <osgViewer/Viewer>
#include <osgGA/NodeTrackerManipulator>

#include "forwarddeclarations.h"

using namespace troen::physics;

class UpdateBikePositionCallback : public osg::NodeCallback
{

public:
	UpdateBikePositionCallback(Bike* bike, osgViewer::View *view);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);

protected:
	osg::Vec3d m_bikePosition;
	osg::ref_ptr<Bike> m_bike;
	osg::ref_ptr<osgViewer::View> m_view;
};