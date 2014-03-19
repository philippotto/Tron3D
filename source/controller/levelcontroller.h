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
		LevelController(TroenGame* troenGame, std::string levelName);

		virtual btTransform getSpawnPointForBikeWithIndex(const int index);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void addItemBox();
		void update();

		btTransform getRandomSpawnPoint();
		osg::ref_ptr<osg::Group>  getFloorView();

		void addRigidBodiesToWorld();
		void removeRigidBodiesFromWorld();
		void reload();

	private:
		std::shared_ptr<LevelView> m_levelView;
		std::shared_ptr<LevelModel> m_levelModel;
		std::vector<btTransform> m_initialBikePositionTransforms;
		virtual void initializeSpawnPoints();
		std::weak_ptr<PhysicsWorld> m_world;

		TroenGame* m_troenGame;
		std::string m_levelName;
		int m_currentItemCount;
		int m_targetItemCount = 100;
	};
}