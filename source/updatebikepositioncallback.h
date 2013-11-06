#include <osg/Vec3>

#include <osg/Node>

#include "forwarddeclarations.h"

class UpdateBikePositionCallback : public osg::NodeCallback
{

public:
	UpdateBikePositionCallback::UpdateBikePositionCallback(BikeInputState* bikeInputState)
		: m_rotation(0.0), m_bikePosition(0.0, 0.0, 0.0)
	{
		m_bikeInputState = bikeInputState;
	}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);

protected:
	osg::Vec3d m_bikePosition;
	double m_rotation;
	BikeInputState* m_bikeInputState;
};