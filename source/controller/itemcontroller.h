#pragma once
// OSG
//bullet
#include <btBulletDynamicsCommon.h>
#include <osg/Vec3>
// troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class ItemController : public AbstractController
	{
	public:
		ItemController(btVector3 position, std::weak_ptr<PhysicsWorld> world, TroenGame* troenGame, LevelView* view);

		osg::Vec3 getDimensions();

		enum Type
		{
			TURBOSTRIP, HEALTHUP, COIN
		};


		void triggerOn(BikeController* bikeController);

	private:
		Type m_type;
		btVector3 m_position;
		void remove();

		TroenGame* m_troenGame;
	};
}