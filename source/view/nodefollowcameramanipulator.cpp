#include "nodefollowcameramanipulator.h"
// OSG
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
// troen
#include "../constants.h"



using namespace troen;

NodeFollowCameraManipulator::NodeFollowCameraManipulator(osg::ref_ptr<osg::Camera> camera) : osgGA::NodeTrackerManipulator()
{
	m_physicsWorld = NULL;
	m_trackBike = NULL;
	m_camera = camera;
	m_rotationMode = TRACKBALL;
	fixedRotation = _rotation;
}

osg::Matrixd NodeFollowCameraManipulator::getMatrix() const
{
	osg::Vec3d nodeCenter;
	osg::Quat nodeRotation;
	computeNodeCenterAndRotation(nodeCenter, nodeRotation);
	osg::Matrixd  transform = osg::Matrixd::translate(0.0, 0.0, _distance)*osg::Matrixd::rotate(_rotation)*osg::Matrixd::rotate(nodeRotation)*osg::Matrix::translate(nodeCenter);
	//osg::Matrixd  transform = osg::Matrixd::translate(0.0, 0.0, _distance)*osg::Matrixd::rotate(_rotation);

	return transform;
}

osg::Matrixd NodeFollowCameraManipulator::getInverseMatrix() const
{
	osg::Vec3d nodeCenter;
	osg::Quat nodeRotation;
	osg::Matrixd transform;
	computeNodeCenterAndRotation(nodeCenter, nodeRotation);
	if (m_rotationMode == TRACKBALL)
		transform = osg::Matrixd::translate(-nodeCenter)*osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0, 0.0, -_distance);
	else
		transform = osg::Matrixd::translate(-nodeCenter)*osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(fixedRotation.inverse())*osg::Matrixd::translate(0.0, 0.0, -_distance);

	//osg::Camera tmpCam = osg::Camera(*m_camera.get());


	return transform;
	//return osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0, 0.0, -_distance);
}
void NodeFollowCameraManipulator::setTrackNode(osg::Node* node, osg::ref_ptr<osg::PositionAttitudeTransform> trackBike)
{
	NodeTrackerManipulator::setTrackNode(node);
	m_trackBike = trackBike;
}



//void NodeFollowCameraManipulator::setTrackPositionAttiduteTransform(osg::ref_ptr<osg::PositionAttitudeTransform> pat)
//{
//	m_trackPAT = pat;
//}

void NodeFollowCameraManipulator::setByMatrix(const osg::Matrixd& matrix)
{
	setByInverseMatrix(osg::Matrixd::inverse(matrix));
}

void NodeFollowCameraManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
	osg::Vec3d eye, center, up;
	matrix.getLookAt(eye, center, up);

	_center = center; _center.z() = 0.0f;
	if (_node.valid())
		_distance = abs((_node->getBound().center() - eye).z());
	else
		_distance = abs((eye - center).length());
}

osg::Matrix NodeFollowCameraManipulator::computeTargetToWorldMatrix(osg::Node* node) const
{
	osg::Matrix l2w;
	if (node && node->getNumParents()>0)
	{
		osg::Group* parent = node->getParent(0);
		l2w = osg::computeLocalToWorld(parent->
			getParentalNodePaths()[0]);
	}
	return l2w;
}

void NodeFollowCameraManipulator::computePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
	if (!_node) return;

	// compute rotation matrix
	osg::Vec3d lv(center - eye);
	_distance = lv.length();

	osg::Matrixd lookat;
	lookat.makeLookAt(eye, center, up);

	_rotation = lookat.getRotate().inverse();
}



void NodeFollowCameraManipulator::computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const
{
	osg::Matrixd localToWorld, worldToLocal;
	computeNodeLocalToWorld(localToWorld);
	computeNodeWorldToLocal(worldToLocal);

	// center
	osg::NodePath nodePath;
	if (_trackNodePath.getNodePath(nodePath) && !nodePath.empty())
	{
		nodeCenter = osg::Vec3d(nodePath.back()->getBound().center())*localToWorld + CAMERA_POSITION_OFFSET;
		//osg::MatrixTransform
		//std::vector <osg::Vec3> nearPlane;
		//getNearPlane(nearPlane,nodeCenter);
		//nodeCenter = handleCollisionZoom(nodeCenter, m_trackPAT->getPosition(), 1.0, nearPlane);
	}
	else
		nodeCenter = osg::Vec3d(0.0f, 0.0f, 0.0f)*localToWorld;
		// rotation
		osg::Matrixd coordinateFrame = getCoordinateFrame(nodeCenter);
		osg::Matrixd localToFrame(localToWorld*osg::Matrixd::inverse(coordinateFrame));

		osg::Quat nodeYawRelToFrame, nodePitchRelToFrame, nodeRollRelToFrame;
		osg::Quat rotationOfFrame;

		double yaw = atan2(-localToFrame(0, 1), localToFrame(0, 0));
		nodeYawRelToFrame.makeRotate(-yaw + CAMERA_ROTATION_OFFSET, osg::Z_AXIS);
	
		double roll = atan2(-localToFrame(0, 2), sqrt(pow(localToFrame(1, 2), 2) + pow(localToFrame(2, 2), 2)));
		nodeRollRelToFrame.makeRotate(roll / CAMERA_TILT_FACTOR,osg::Y_AXIS);

		// jd: camera pitch rotation not wanted so far, maybe useful for later
		//double pitch = atan2(localToFrame(1, 2), localToFrame(2, 2));
		//nodePitchRelToFrame.makeRotate(pitch, osg::X_AXIS);
	
		rotationOfFrame = coordinateFrame.getRotate();
		nodeRotation = nodeRollRelToFrame*nodeYawRelToFrame*rotationOfFrame;
}

void NodeFollowCameraManipulator::setPhysicsWord(std::shared_ptr<PhysicsWorld> physics)
{
	m_physicsWorld = physics;
}


osg::Vec3 NodeFollowCameraManipulator::keepCameraOverGround(osg::Vec3 camPos, osg::Vec3 targetPos) const
{
	//somehow these two vakues reside in different coordinate systems
	if (camPos.z() < targetPos.z())
	{
		return osg::Vec3(camPos.x(), camPos.y(),targetPos.z());
	}
	else
		return camPos ;

	//return osg::Vec3(camPos.x(), camPos.y(), 100.0);
}



// returns a new camera position
osg::Vec3 NodeFollowCameraManipulator::handleCollisionZoom(osg::Vec3 camPos, osg::Vec3 targetPos,
	float minOffsetDist, std::vector<osg::Vec3> frustumNearCorners) const
{
	float offsetDist = (targetPos - camPos).length();
	float raycastLength = offsetDist - minOffsetDist;
	if (raycastLength < 0.f)
	{
		// camera is already too near the lookat target
		return camPos;
	}

	//vector cam to target
	osg::Vec3 camOut = (targetPos - camPos);
	camOut.normalize();
	//nearest possible camera to target
	osg::Vec3 nearestCamPos = targetPos - camOut * minOffsetDist;
	float minHitFraction = 1.0;


	for (int i = 0; i < 4; i++)
	{
	osg::Vec3 corner =  frustumNearCorners[i];
		//vector cam to corner
		osg::Vec3 offsetToCorner = corner - camPos;
		//corner of nearest nearplane
		osg::Vec3 rayStart = nearestCamPos +offsetToCorner;
		osg::Vec3 rayEnd = corner;
		// a result between 0 and 1 indicates a hit along the ray segment, store if hit occurs
		float hitFraction = m_physicsWorld->RayTest_GetHitFraction(btVector3(rayStart.x(), rayStart.y(), rayStart.z()), btVector3(rayEnd.x(), rayEnd.y(), rayEnd.z()));
		minHitFraction = std::min(hitFraction, minHitFraction);
	}

	if (minHitFraction < 1.0)
	{
		return nearestCamPos - camOut * (raycastLength * minHitFraction);
	}
	else
	{
		return camPos;
	}
}


void NodeFollowCameraManipulator::getNearPlane(std::vector<osg::Vec3>& v, osg::Vec3d& nodeCenter)  const
{
	osg::Matrixd proj = m_camera->getProjectionMatrix();
	//osg::Matrixd view_proj_inv = osg::Matrixd::inverse(m_camera->getViewMatrix() * m_camera->getProjectionMatrix());
	// Get near and far from the Projection matrix.
	const double near = proj(3, 2) / (proj(2, 2) - 1.0);

	// Get the sides of the near plane.
	const double nLeft = near * (proj(2, 0) - 1.0) / proj(0, 0);
	const double nRight = near * (1.0 + proj(2, 0)) / proj(0, 0);
	const double nTop = near * (1.0 + proj(2, 1)) / proj(1, 1);
	const double nBottom = near * (proj(2, 1) - 1.0) / proj(1, 1);
	
	//v.clear();
	//v.push_back(osg::Vec3(nLeft, nBottom, -near));
	//v.push_back(osg::Vec3(nRight, nBottom, -near));
	//v.push_back(osg::Vec3(nRight, nTop, -near));
	//v.push_back(osg::Vec3(nLeft, nTop, -near));

	v.clear();
	v.push_back(osg::Vec3(-0.1, -0.1, -near) + nodeCenter);
	v.push_back(osg::Vec3(.1, -.1, -near) + nodeCenter);
	v.push_back(osg::Vec3(.1, .1, -near) + nodeCenter);
	v.push_back(osg::Vec3(-.1, .1, -near) + nodeCenter);

}