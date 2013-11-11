#pragma once

#include <memory>

#include "abstractcontroller.h"
#include <osg/Referenced>
#include "../forwarddeclarations.h"

#include "../view/bikeview.h"
#include "../model/bikemodel.h"

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