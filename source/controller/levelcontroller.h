#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "abstractcontroller.h"
#include "../forwarddeclarations.h"

namespace troen
{
	class LevelController : public AbstractController
	{
	public:
		LevelController();


		virtual btTransform getSpawnPointForBikeWithIndex(const int index);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void addItemBox(btVector3& position);
		btTransform getRandomSpawnPoint();
		osg::ref_ptr<osg::Group>  getFloorView();

	private:
		std::vector<btTransform> m_initialBikePositionTransforms;
		virtual void initializeSpawnPoints();
		std::weak_ptr<PhysicsWorld> m_world;
	};
}