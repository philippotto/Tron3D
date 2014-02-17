#pragma once
// OSG
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	class NodeFollowCameraManipulator : public osgGA::NodeTrackerManipulator
	{
	public:
		virtual osg::Matrixd getMatrix() const;
		virtual osg::Matrixd getInverseMatrix() const;
		virtual void setByMatrix(const osg::Matrixd& matrix);
		virtual void setByInverseMatrix(const osg::Matrixd& matrix);

		virtual void computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const;

		virtual void setBikeInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);

	protected:
		osg::Vec3 rollPitchYaw(float x, float y, float z, float w) const;
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
	};
}