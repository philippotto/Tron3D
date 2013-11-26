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

			typedef enum e_COLLISIONTYPE
			{
				ABSTRACTTYPE,
				BIKETYPE,
				LEVELTYPE,
				LEVELWALLTYPE,
				LEVELFLOORTYPE,
				FENCETYPE
			} COLLISIONTYPE;

			virtual osg::ref_ptr<osg::Group> getViewNode();
			virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();
			virtual const COLLISIONTYPE getCollisionType() { return ABSTRACTTYPE; };

		protected:
			std::shared_ptr<AbstractView> m_view;
			std::shared_ptr<AbstractModel> m_model;
	};
}