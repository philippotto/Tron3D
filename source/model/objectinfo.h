#pragma once
//troen
#include "../controller/abstractcontroller.h"

namespace troen {
	/*! The ObjectInfo class is a helper class whose instances get referred from the userPointer-attribute of Bullet’s CollisionBodies. The class can hold a pointer and an integer which are used for classifying collisions. The pointer is used for getting a Troen-Controller and the integer is used for getting the type of the collision (e.g. wall-collision).
*/
	class ObjectInfo
	{
	public:
		ObjectInfo(AbstractController* userPointer, const int userIndex) {
			m_userPointer = userPointer;
			m_userIndex = userIndex;
		};

		AbstractController* getUserPointer() {
			return m_userPointer;
		}

		int getUserIndex() {
			return m_userIndex;
		}

	private:
		AbstractController* m_userPointer;
		int m_userIndex;
	};
}