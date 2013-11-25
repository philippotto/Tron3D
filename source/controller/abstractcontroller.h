#pragma once
// OSG
#include <osg/ref_ptr>
// STD
#include <vector>
#include <memory>
// troen
#include "../forwarddeclarations.h"


namespace troen
{
	class AbstractController
	{
		public:
			AbstractController();

			typedef enum e_CONTROLLERTYPE
			{
				ABSTRACTCONTROLLER,
				BIKECONTROLLER,
				LEVELCONTROLLER,
				FENCECONTROLLER
			} CONTROLLERTYPE;

			virtual osg::ref_ptr<osg::Group> getViewNode();
			virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();
			virtual const CONTROLLERTYPE getType() { return ABSTRACTCONTROLLER; };

		protected:
			std::shared_ptr<AbstractView> m_view;
			std::shared_ptr<AbstractModel> m_model;
	};
}