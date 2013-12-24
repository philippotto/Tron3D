#pragma once
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class FenceModel : public AbstractModel
	{
	public:
		FenceModel(FenceController* fenceController, int maxFenceParts);
		void attachWorld(std::shared_ptr<PhysicsWorld>& world);

		float getFenceHeight();

		void addFencePart(btVector3 a, btVector3 b);
		void removeFirstFencePart();
		void removeAllFences();
		void enforceFencePartsLimit(int maxFenceParts);

	private:
		FenceController* m_fenceController;
		std::weak_ptr<PhysicsWorld> m_world;

		std::deque<std::shared_ptr<btRigidBody>>		m_rigidBodyDeque;
		std::deque<std::shared_ptr<btMotionState>>		m_motionStateDeque;
		std::deque<std::shared_ptr<btCollisionShape>>	m_collisionShapeDeque;

		int m_maxFenceParts;
	};
}