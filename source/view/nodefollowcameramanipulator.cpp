#include "nodefollowcameramanipulator.h"

#include <osg/PositionAttitudeTransform>

using namespace troen;


osg::Matrixd NodeFollowCameraManipulator::getMatrix() const
{
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

void NodeFollowCameraManipulator::computeNodeCenterAndRotation(osg::Vec3d& nodeCenter, osg::Quat& nodeRotation) const
{
	osg::Vec3 positionOffset = osg::Vec3(0, 0, 25);
	float rotationOffset = .01;

	osg::Matrixd localToWorld, worldToLocal;
	computeNodeLocalToWorld(localToWorld);
	computeNodeWorldToLocal(worldToLocal);

	osg::NodePath nodePath;
	if (_trackNodePath.getNodePath(nodePath) && !nodePath.empty())
	{
		nodeCenter = osg::Vec3d(nodePath.back()->getBound().center())*localToWorld + positionOffset;
	}
	else
		nodeCenter = osg::Vec3d(0.0f, 0.0f, 0.0f)*localToWorld;

	osg::Matrixd coordinateFrame = getCoordinateFrame(nodeCenter);
	osg::Matrixd localToFrame(localToWorld*osg::Matrixd::inverse(coordinateFrame));

	double azim = atan2(-localToFrame(0, 1), localToFrame(0, 0));
	osg::Quat nodeRotationRelToFrame, rotationOfFrame;
	nodeRotationRelToFrame.makeRotate(-azim + rotationOffset, 0.0, 0.0, 1.0);
	rotationOfFrame = coordinateFrame.getRotate();
	nodeRotation = nodeRotationRelToFrame*rotationOfFrame;
}