#pragma once
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class ItemModel : public AbstractModel
	{
	public:
		ItemModel(btVector3 dimensions, btVector3 position, std::weak_ptr<PhysicsWorld> world, ItemController* itemController);

		void remove();
	private:
		btCollisionObject* m_pTrigger;
		std::weak_ptr<PhysicsWorld> m_world;

	};
}