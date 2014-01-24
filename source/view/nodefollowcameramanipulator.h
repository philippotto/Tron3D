#pragma once
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>

#include <OVR.h>

namespace troen
{
	class NodeFollowCameraManipulator : public osgGA::NodeTrackerManipulator
	{
	public:
		NodeFollowCameraManipulator::NodeFollowCameraManipulator(OVR::SensorFusion* SFusion = nullptr) : osgGA::NodeTrackerManipulator()
		{

			m_SFusion = SFusion;
		}

		virtual osg::Matrixd getMatrix() const;
		virtual osg::Matrixd getInverseMatrix() const;
		virtual void setByMatrix(const osg::Matrixd& matrix);
		virtual void setByInverseMatrix(const osg::Matrixd& matrix);

		virtual void computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const;

	protected:
		osg::Vec3 rollPitchYaw(float x, float y, float z, float w) const;

	private:
		OVR::SensorFusion* m_SFusion;

	};
}