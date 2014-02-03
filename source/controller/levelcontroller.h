#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "abstractcontroller.h"
#include "../forwarddeclarations.h"
#include "itemcontroller.h"

namespace troen
{
	class LevelController : public AbstractController
	{
	public:
		LevelController();

		virtual btTransform getSpawnPointForBikeWithIndex(int index);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void addItemBox();

		void update();

		btTransform getRandomSpawnPoint();

		int m_currentItemCount;
		int m_targetItemCount = 100;

	private:
		std::vector<btTransform> m_initialBikePositionTransforms;
		virtual void initializeSpawnPoints();
		std::weak_ptr<PhysicsWorld> m_world;
	};
}