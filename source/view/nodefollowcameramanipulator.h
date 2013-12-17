#pragma once
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>

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

	protected:
		osg::Vec3 rollPitchYaw(float x, float y, float z, float w) const;

	};
}