#pragma once
//troen
#include "../controller/abstractcontroller.h"

namespace troen {
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