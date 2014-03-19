#pragma once
// OSG
// troen
#include "troengame.h"

namespace troen
{
	class TroenGameBuilder
	{

	public:
		TroenGameBuilder(TroenGame * game);

		bool build();
		bool destroy();
		bool setupReflections();

	private:
		bool buildInput();
		bool composeSceneGraph();
		bool buildGameLogic();
		bool buildPhysicsWorld();

		TroenGame * t;
	};
}