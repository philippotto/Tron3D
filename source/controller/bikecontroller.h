#pragma once
// STD
#include <memory>
// OSG
#include <osg/Referenced>
// troen
#include "../forwarddeclarations.h"
#include "../view/bikeview.h"
#include "../model/bikemodel.h"
#include "abstractcontroller.h"

namespace troen
{
	class BikeController : public AbstractController
	{
	public:
		BikeController();

	private:
		std::shared_ptr<BikeView> m_view;
		std::shared_ptr<BikeModel> m_model;
	};
}