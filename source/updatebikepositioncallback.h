#include <osg/Vec3>

#include <osg/Node>

#include "forwarddeclarations.h"

class UpdateBikePositionCallback : public osg::NodeCallback
{

public:
	UpdateBikePositionCallback(BikeInputState* bikeInputState);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);

protected:
	osg::Vec3d m_bikePosition;
	double m_rotation;
	osg::ref_ptr<BikeInputState> m_bikeInputState;
};