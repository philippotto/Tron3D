#pragma once
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class HUDController : public AbstractController
	{
	public:
		HUDController();

		void attachSceneToRadarCamera(osg::Group* scene);
		void resize(int width, int height);
	};
}