#include "nodefollowcameramanipulator.h"

using namespace troen;

void NodeFollowCameraManipulator::setByMatrix( const osg::Matrixd& matrix )
{
	osg::Vec3d eye, center, up;
	matrix.getLookAt(eye, center, up, _distance);
	computePosition(eye, center, up);
}

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

void NodeFollowCameraManipulator::update(osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection)
{
	upDirection.normalize();
	nodeMoveDirection.normalize();
	osg::Vec3f camPos = position + upDirection * 3.0f;
	osg::Vec3f center  = position + nodeMoveDirection + upDirection * 3.0f;
	m_lastViewMatrix.makeLookAt(camPos, center, upDirection);
}

void NodeFollowCameraManipulator::update( osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection, double speed)
{
	// make distance smaller for "Field of View" effect
	float distance;
	if (speed > 210.0)
		distance = m_distance * (0.36-((speed-210)*0.003));
	else if (speed > 10.0)
		distance = m_distance * (1-((speed-10)*0.0032));
	else
		distance = m_distance;

	nodeMoveDirection.normalize();
	osg::Vec3f cameraPosition = position - (nodeMoveDirection * distance) + upDirection * ((m_distance*0.6+ + distance*0.7) / 5.5);
	m_lastViewMatrix.makeLookAt(cameraPosition, position, upDirection);
}

void NodeFollowCameraManipulator::updateWithFixPosition( osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection, double speed)
{
	osg::Vec3f cameraPosition = osg::Vec3f(-50.0, 40.0, 450.0);
	upDirection = osg::Vec3f(0.0, 1.0, 0.0);
	m_lastViewMatrix.makeLookAt(cameraPosition, position, upDirection);
}
