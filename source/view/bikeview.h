#pragma once
// OSG
#include <osg/Referenced>
// STD
#include <memory>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"


namespace troen
{

	class BikeView : public AbstractView //, public osg::Referenced
	{
	public:
		BikeView(std::shared_ptr<BikeModel> model);

	private:
		std::shared_ptr<BikeModel> m_model;

	};
}