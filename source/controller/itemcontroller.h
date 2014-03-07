#pragma once
// qt
#include <QObject>
//bullet
#include <btBulletDynamicsCommon.h>
#include <osg/Vec3>
// troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class ItemController : public QObject, public AbstractController
	{
		Q_OBJECT
	public:
		ItemController(btVector3 position, std::weak_ptr<PhysicsWorld> world, LevelView* view);

		osg::Vec3 getDimensions();

		enum Type
		{
			TURBOSTRIP, HEALTHUP, RADAR, COUNT
		};

		void triggerOn(BikeController* bikeController);

	public slots:
		void hideFencesInRadarForPlayer();

	private:
		std::shared_ptr<ItemView> m_itemView;
		std::shared_ptr<ItemModel> m_itemModel;
		Type m_type;
		btVector3 m_position;
		int m_id;
		BikeController* m_bikeController;
		void remove();
	};
}