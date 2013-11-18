// STD
#include <memory>
//OSG
#include <osg/Vec3>
#include <osg/Node>
#include <osg/DisplaySettings>
#include <osgViewer/Viewer>
#include <osgGA/NodeTrackerManipulator>
// troen
#include "forwarddeclarations.h"

namespace troen
{
	class UpdateBikePositionCallback : public osg::NodeCallback
	{
	public:
		UpdateBikePositionCallback(std::shared_ptr<BikeModel> bike);
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);

	protected:
		osg::Vec3d m_bikePosition;
		std::shared_ptr<BikeModel> m_bike;
	};
}