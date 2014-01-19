#pragma once


class ObjectInfo
{
public:
	ObjectInfo(void* userPointer, int userIndex) {
		m_userPointer = userPointer;
		m_userIndex = userIndex;
	};

	void* getUserPointer() {
		return m_userPointer;
	}

	int getUserIndex() {
		return m_userIndex;
	}

private:
	void* m_userPointer;
	int m_userIndex;
};