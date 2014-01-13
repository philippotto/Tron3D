#pragma once
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>

#include "../model/physicsworld.h"
#include "../controller/bikecontroller.h"
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
		void setTrackNode(osg::Node* node, osg::ref_ptr<osg::PositionAttitudeTransform> trackBike);
		void getNearPlane(std::vector<osg::Vec3>& v, osg::Vec3d& nodeCenter)  const;

		virtual void computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const;
		osg::Vec3 handleCollisionZoom(osg::Vec3 camPos, osg::Vec3 targetPos,
			float minOffsetDist, std::vector<osg::Vec3> frustumNearCorners) const;
		osg::Vec3 keepCameraOverGround(osg::Vec3 camPos, osg::Vec3 targetPos) const;
		void setPhysicsWord(std::shared_ptr<PhysicsWorld> physics);
		osg::Matrix computeTargetToWorldMatrix(osg::Node* node) const;
		void computePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up);
		/*void setTrackPositionAttiduteTransform(osg::ref_ptr<osg::PositionAttitudeTransform> pat);*/

		void setHomePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up, bool autoComputeHomePosition = false);

		int m_rotationMode;
		osg::Quat fixedRotation;

		enum rotationMode
		{
			TRACKBALL,
			FIXEDCAMERA
		};


	protected:
		osg::Vec3 rollPitchYaw(float x, float y, float z, float w) const;
		std::shared_ptr<PhysicsWorld>  m_physicsWorld;
		osg::ref_ptr<osg::PositionAttitudeTransform> m_trackBike;
		osg::ref_ptr<osg::Camera> m_camera;

		osg::Vec3 *m_cachedCenter = new osg::Vec3(0.0, 0.0, 0.0);
		osg::Vec3 *m_cachedEye = new osg::Vec3(0.0, 0.0, 10.0);
		osg::Vec3 *m_cachedUp = new osg::Vec3(0.0, 0.0, 0.0);
		

	};
}