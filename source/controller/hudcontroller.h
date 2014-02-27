#pragma once
//osg
#include <osg/Array>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"
#include "../gamelogic.h"

namespace troen
{
	class HUDController : public AbstractController
	{
	public:
		HUDController(const int index, const std::vector<std::shared_ptr<Player>>& players);
		
		void attachSceneToRadarCamera(osg::Group* scene);
		void resize(const int width, const int height);
		void update(
			const long double currentGameloopTime,
			const long double currentGameTime,
			const int timeLimit,
			const GameLogic::GAMESTATE gameState,
			const std::vector<std::shared_ptr<Player>>& players);
        
		void setTrackNode(osg::Node* trackNode);
	private:
		std::weak_ptr<Player> m_player;
	};
}