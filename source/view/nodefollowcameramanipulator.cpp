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
	osg::Matrixd localToWorld, worldToLocal;
	computeNodeLocalToWorld(localToWorld);
	computeNodeWorldToLocal(worldToLocal);

	osg::NodePath nodePath;
	if (_trackNodePath.getNodePath(nodePath) && !nodePath.empty())
	{
		osg::Vec3 offset = osg::Vec3(0, 0, 25);
		nodeCenter = osg::Vec3d(nodePath.back()->getBound().center())*localToWorld + offset;
	}
	else
		nodeCenter = osg::Vec3d(0.0f, 0.0f, 0.0f)*localToWorld;


	// scale the matrix to get rid of any scales before we extract the rotation.
	double sx = 1.0 / sqrt(localToWorld(0, 0)*localToWorld(0, 0) + localToWorld(1, 0)*localToWorld(1, 0) + localToWorld(2, 0)*localToWorld(2, 0));
	double sy = 1.0 / sqrt(localToWorld(0, 1)*localToWorld(0, 1) + localToWorld(1, 1)*localToWorld(1, 1) + localToWorld(2, 1)*localToWorld(2, 1));
	double sz = 1.0 / sqrt(localToWorld(0, 2)*localToWorld(0, 2) + localToWorld(1, 2)*localToWorld(1, 2) + localToWorld(2, 2)*localToWorld(2, 2));
	localToWorld = localToWorld*osg::Matrixd::scale(sx, sy, sz);

	osg::Quat rot = localToWorld.getRotate();
	
	//osg::Vec3 rollPitchYaw = this->rollPitchYaw(rot.x(), rot.y(), rot.z(), rot.w());

	//rollPitchYaw.x

	nodeRotation = localToWorld.getRotate();

}

//osg::Vec3 NodeFollowCameraManipulator::rollPitchYaw(float x, float y, float z, float w) const
//{
//	float roll = atan2(2 * y*w - 2 * x*z, 1 - 2 * y*y - 2 * z*z);
//	float pitch = atan2(2 * x*w - 2 * y*z, 1 - 2 * x*x - 2 * z*z);
//	float yaw = asin(2 * x*y + 2 * z*w);
//
//	return osg::Vec3(roll, pitch, yaw);
//}