#pragma once
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"

#include <hidapi.h>

namespace troen
{
namespace input
{
	class GamepadPS4 : public PollingDevice
	{
	public:
		GamepadPS4(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState) {
			m_deadzoneX = 0.25f;
			m_deadzoneY = 0.02f;
		};
		~GamepadPS4();
		enum MOVEDIRECTION { LEFT, RIGHT, FORWARD, BACKWARD };
		enum PS4KEY {
			UP_PRESSED, DOWN_PRESSED, LEFT_PRESSED, RIGHT_PRESSED, ONE_PRESSED, TWO_PRESSED, THREE_PRESSED, FOUR_PRESSED, LEFT_1_PRESSED,
			LEFT_2_PRESSED, RIGHT_1_PRESSED, RIGHT_2_PRESSED, SHARE_PRESSED, OPTION_PRESSED, LEFT_HAT_PRESSED, RIGHT_HAT_PRESSED, LEFT_HAT_X, LEFT_HAT_Y, RIGHT_HAT_Y, RIGHT_HAT_X
		};
		bool refresh();
		bool checkConnection();
		hid_device *_controller;

	private:
		int getBitAt(int k, unsigned char * buf);
		int getValueFromKey(PS4KEY type, unsigned char *buffer);
		void printBinary(unsigned char *a, int iStart = 0, int iEnd = 8);
		int calcDecimalFromBinar(unsigned char *binar, int i, int jMax);

		float m_deadzoneX, m_deadzoneY;

	};
}
}