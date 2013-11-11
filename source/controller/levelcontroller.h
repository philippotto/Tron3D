#pragma once


#include <memory>
#include <osg/ref_ptr>

#include "abstractcontroller.h"
#include "../forwarddeclarations.h"



namespace troen
{
	class LevelController : public AbstractController
	{
	public:
		LevelController();

		osg::ref_ptr<osg::Group> getViewNode();
		// std::shared_ptr<LevelView> getModel();

	private:
		std::shared_ptr<LevelView> m_view;
		std::shared_ptr<LevelModel> m_model;
	};
}