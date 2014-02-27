#pragma once
// OSG
#include <osg/Referenced>
// troen
#include "../forwarddeclarations.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"

namespace troen
{
namespace input
{
	class BikeInputState : public osg::Referenced
	{
	public:
		enum InputDevice
		{
			KEYBOARD_wasd, KEYBOARD_arrows, GAMEPAD, GAMEPADPS4, AI, REMOTE_PLAYER
		};

		BikeInputState();
		float getAngle();
		float getAcceleration();
		void setAngle(const float angle);
		void setAcceleration(const float acceleration);
		void setTurboPressed(const bool pressed);
		bool getTurboPressed();
		void setViewingAngle(float angle);
		float getViewingAngle();
		void setRemote(bool isRemote);
		bool isRemote();
		btVector3 getPosition();
		void setPosition(btVector3 position);
		btQuaternion getRotation();
		void setRotation(btQuaternion val);
	private:
		float m_acceleration;
		float m_angle;
		bool m_turboPressed;
		float m_viewingAngle;
		bool m_isRemote;
		btVector3 m_position;
		btQuaternion m_rotation;
	};
}
}