#include "nodefollowcameramanipulator.h"
// OSG
#include <osg/PositionAttitudeTransform>
// troen
#include "../constants.h"
#include "../input/bikeinputstate.h"

using namespace troen;


osg::Matrixd NodeFollowCameraManipulator::getMatrix() const
{
	NodeTrackerManipulator();
	osg::Vec3d nodeCenter;
	osg::Quat nodeRotation;
	computeNodeCenterAndRotation(nodeCenter, nodeRotation);
	return osg::Matrixd::translate(0.0, 0.0, _distance)*osg::Matrixd::rotate(_rotation)*osg::Matrixd::rotate(nodeRotation)*osg::Matrix::translate(nodeCenter);
}

osg::Matrixd NodeFollowCameraManipulator::getInverseMatrix() const
{
	osg::Vec3d nodeCenter;
	osg::Quat nodeRotation;
	computeNodeCenterAndRotation(nodeCenter, nodeRotation);
	return osg::Matrixd::translate(-nodeCenter)*osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0, 0.0, -_distance);
}

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

void NodeFollowCameraManipulator::setBikeInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_oldPlayerViewingRotation.makeRotate(0, osg::Z_AXIS);
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

	osg::Quat playerViewingRotation;
	float angle = m_bikeInputState->getViewingAngle();
	playerViewingRotation.makeRotate(angle, osg::Z_AXIS);

	// do spherical-linear interpolation (slerp) between the old and new viewing direction
	playerViewingRotation.slerp(BIKE_VIEWING_ANGLE_DAMPENING_TERM, m_oldPlayerViewingRotation, playerViewingRotation);
	m_oldPlayerViewingRotation = playerViewingRotation;

	rotationOfFrame = coordinateFrame.getRotate();
	nodeRotation = playerViewingRotation*nodeRollRelToFrame*nodeYawRelToFrame*rotationOfFrame;
}