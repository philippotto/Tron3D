#pragma once

#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include "../controller/fencecontroller.h"

namespace troen
{
	class BikeMotionState : public btMotionState 
	{
	public:
		BikeMotionState(const btTransform &initialpos, osg::PositionAttitudeTransform* pat, std::shared_ptr<FenceController> fenceController) {
			m_visibleObj = pat;
			mPos1 = initialpos;
			m_fenceController = fenceController;
		}

		virtual ~BikeMotionState() {
		}

		void setNode(osg::PositionAttitudeTransform* pat) {
			m_visibleObj = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = mPos1;
		}

		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (NULL == m_visibleObj)
				return; // silently return before we set a node
			btQuaternion rot = worldTrans.getRotation();
			osg::Vec3 axis = osg::Vec3(rot.getAxis().x(), rot.getAxis().y(), rot.getAxis().z());
			osg::Quat rotationQuat(rot.getAngle(), axis);
			m_visibleObj->setAttitude(rotationQuat);

			btVector3 pos = worldTrans.getOrigin();
			m_visibleObj->setPosition(osg::Vec3(pos.x(), pos.y(), pos.z()));

			// update fence accordingly
			m_fenceController->update(osg::Vec3(pos.x(), pos.y(), pos.z()));
		}

	protected:
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform mPos1;
		std::shared_ptr<FenceController> m_fenceController;
	};
}