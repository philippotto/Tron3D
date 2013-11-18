#pragma once

#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>

namespace troen
{
	class BikeMotionState : public btMotionState 
	{
	public:
		BikeMotionState(const btTransform &initialpos, osg::PositionAttitudeTransform* pat) {
			mVisibleobj = pat;
			mPos1 = initialpos;
		}

		virtual ~BikeMotionState() {
		}

		void setNode(osg::PositionAttitudeTransform* pat) {
			mVisibleobj = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = mPos1;
		}

		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (NULL == mVisibleobj)
				return; // silently return before we set a node
			btQuaternion rot = worldTrans.getRotation();
			osg::Vec3 axis = osg::Vec3(rot.getAxis().x(), rot.getAxis().y(), rot.getAxis().z());
			osg::Quat rotationQuat(rot.getAngle(), axis);
			mVisibleobj->setAttitude(rotationQuat);

			btVector3 pos = worldTrans.getOrigin();
			mVisibleobj->setPosition(osg::Vec3(pos.x(), pos.y(), pos.z()));
		}

	protected:
		osg::PositionAttitudeTransform* mVisibleobj;
		btTransform mPos1;
	};
}