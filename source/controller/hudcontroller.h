#pragma once
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class HUDController : public AbstractController
	{
	public:

		HUDController(std::shared_ptr<BikeController> bikeController);
		void attachSceneToRadarCamera(osg::Group* scene);
		void resize(int width, int height);
		void update();

	private:
		std::weak_ptr<BikeController> m_bikeController;
	};
}