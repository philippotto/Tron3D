#pragma once
//osg
#include <osg/Array>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class HUDController : public AbstractController
	{
	public:

		HUDController(std::shared_ptr<BikeController> bikeController, const osg::Vec4 playerColor);
		void attachSceneToRadarCamera(osg::Group* scene);
		void resize(const int width, const int height);
		void update(const double currentTime);
        void setTrackNode(osg::Node* trackNode);
		std::weak_ptr<BikeController> getBikeController();
	private:
		std::weak_ptr<BikeController> m_bikeController;
	};
}