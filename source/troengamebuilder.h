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

	private:
		bool buildInput();
		bool composeSceneGraph();
		bool buildGameLogic();
		bool buildPhysicsWorld();
		bool setupNetworking();
		TroenGame * t;
	};
}