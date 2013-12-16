#pragma once
#include <osgGA/NodeTrackerManipulator>
#include <osg/Matrixd>

namespace troen
{
	class NodeFollowCameraManipulator : public osgGA::NodeTrackerManipulator
	{
	public:
		NodeFollowCameraManipulator(float distanceToNode) : m_distance(distanceToNode) {};

		//virtual void setByMatrix( const osg::Matrixd& matrix );

		//virtual void setByInverseMatrix( const osg::Matrixd& matrix );

		//virtual osg::Matrixd getMatrix() const;

		//virtual osg::Matrixd getInverseMatrix() const;

	//public slots:
		void update(osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection);
		void update(osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection, double speed);
		void updateWithFixPosition(osg::Vec3f nodeMoveDirection, osg::Vec3f position, osg::Vec3f upDirection, double speed);

	private:
		osg::Matrixd m_lastViewMatrix;
		float m_distance;

	};
}