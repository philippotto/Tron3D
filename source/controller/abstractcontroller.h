#pragma once
// OSG
#include <osg/ref_ptr>
// STD
#include <vector>
// troen
#include "../forwarddeclarations.h"


namespace troen
{
	class AbstractController
	{
		public:
			AbstractController();

			virtual osg::ref_ptr<osg::Group> getViewNode();
			virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();

		protected:
			std::shared_ptr<AbstractView> m_view;
			std::shared_ptr<AbstractModel> m_model;
	};
}