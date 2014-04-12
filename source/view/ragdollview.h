#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>

#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedQuaternionElement>
#include <osgAnimation/BasicAnimationManager>
#include <osg/LineWidth>
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
		osgAnimation::Bone* createBone(const char* name, const osg::Matrix& transform, osg::Group* parent);

		osg::ref_ptr<osgAnimation::Skeleton> getSkelRoot()
		{
			return skelroot;
		}

	private:
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit);
		osgAnimation::Bone* createEndBone(const char* name, const osg::Matrix& transform, osg::Group* parent);
		osgAnimation::Channel* createChannel(const char* name, const osg::Vec3& axis, float rad);
		osg::Geode* createBoneShape(const osg::Vec3& trans, const osg::Vec4& color);
		osg::ref_ptr<osg::MatrixTransform> m_matrixTransform;
		LevelView* m_levelView;
		osg::ref_ptr<osgAnimation::Skeleton> skelroot;
	};
}