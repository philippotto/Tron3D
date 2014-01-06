#pragma once
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>

#include "../model/physicsworld.h"
#include <btBulletDynamicsCommon.h>

namespace troen
{
	class NodeFollowCameraManipulator : public osgGA::NodeTrackerManipulator
	{
	public:
		NodeFollowCameraManipulator(osg::ref_ptr<osg::Camera> camera);
		virtual osg::Matrixd getMatrix() const;
		virtual osg::Matrixd getInverseMatrix() const;
		virtual void setByMatrix(const osg::Matrixd& matrix);
		virtual void setByInverseMatrix(const osg::Matrixd& matrix);
		void setTrackNode(osg::Node* node);
		void getNearPlane(std::vector<osg::Vec3>& v)  const;

		virtual void computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const;
		osg::Vec3 handleCollisionZoom(osg::Vec3 camPos, osg::Vec3 targetPos,
			float minOffsetDist, std::vector<osg::Vec3> frustumNearCorners) const;
		void setPhysicsWord(std::shared_ptr<PhysicsWorld> physics);
		/*void setTrackPositionAttiduteTransform(osg::ref_ptr<osg::PositionAttitudeTransform> pat);*/

	protected:
		osg::Vec3 rollPitchYaw(float x, float y, float z, float w) const;
		std::shared_ptr<PhysicsWorld>  m_physicsWorld;
		osg::ref_ptr<osg::PositionAttitudeTransform> m_trackPAT;
		osg::ref_ptr<osg::Camera> m_camera;

	};
}