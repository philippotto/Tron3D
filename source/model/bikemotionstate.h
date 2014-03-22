#pragma once
#include <algorithm>
// OSG
#include <osg/PositionAttitudeTransform>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../player.h"
#include "../controller/fencecontroller.h"
#include "../model/bikemodel.h"

namespace troen
{
	class BikeMotionState : public btMotionState
	{
	public:
		BikeMotionState(
			const btTransform &initialTransform,
			osg::PositionAttitudeTransform* pat,
			Player * player,
			BikeModel *bikeModel) :
                btMotionState(),
				m_player(player),
                m_bikeModel(bikeModel),
				m_visibleObj(pat),
                m_positionTransform(initialTransform),
				m_currentSteeringTilt(0)
		{}

		virtual ~BikeMotionState() {}

		void setRigidBody(std::weak_ptr<btRigidBody> bikeRigidBody)
		{
			m_rigidBody = bikeRigidBody;
		}

		void setNode(osg::PositionAttitudeTransform* pat) {
			m_visibleObj = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = m_positionTransform;
		}


		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (nullptr == m_visibleObj)
				return; // silently return before we set a node
			btVector3 pos = worldTrans.getOrigin();
			m_currentPosition = osg::Vec3(pos.x(), pos.y(), pos.z());
			m_player->bikeController()->updateView(worldTrans);

			

		}


		osg::Vec3 getPosition()
		{
			return m_currentPosition;
		}
		void setCurrentPosition(osg::Vec3 val) { m_currentPosition = val; }

	protected:
		Player * m_player;
		BikeModel* m_bikeModel;
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform m_positionTransform;
		std::weak_ptr<btRigidBody> m_rigidBody;
		float m_currentSteeringTilt;
		float m_currentWheelyTilt;
		osg::Vec3 m_currentPosition;

	};
}