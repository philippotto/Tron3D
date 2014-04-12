#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../controller/itemcontroller.h"

namespace troen
{
	class RagdollView : public AbstractView
	{
	public:
		RagdollView(osg::Vec3 dimensions, osg::Vec3 position);
		void remove();
		void updateBonePositions();
		osg::ref_ptr<osg::PositionAttitudeTransform> createBodyPart(btTransform transform, float radius, float height);
	private:
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit);
		osg::ref_ptr<osg::MatrixTransform> m_matrixTransform;
		LevelView* m_levelView;
	};
}