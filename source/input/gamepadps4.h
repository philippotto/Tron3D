#pragma once
// OSG
#include <osg/Vec3>
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"

#include <hidapi.h>

namespace troen
{
namespace input
{
/*! The GamepadPS4 class is responsible for receiving input from a Playstation 4 controller. Additionally, it can control the vibration and LED of the controller.*/
	class GamepadPS4 : public PollingDevice
	{
	public:
		GamepadPS4(osg::ref_ptr<BikeInputState> bikeInputState, osg::Vec3 color);
		~GamepadPS4();
		enum PS4KEY {
			UP_PRESSED, DOWN_PRESSED, LEFT_PRESSED, RIGHT_PRESSED, ONE_PRESSED, TWO_PRESSED, THREE_PRESSED, FOUR_PRESSED, LEFT_1_PRESSED,
			LEFT_2_PRESSED, RIGHT_1_PRESSED, RIGHT_2_PRESSED, SHARE_PRESSED, OPTION_PRESSED, LEFT_HAT_PRESSED, RIGHT_HAT_PRESSED, LEFT_HAT_X,
			LEFT_HAT_Y, RIGHT_HAT_Y, RIGHT_HAT_X, LEFT_2, RIGHT_2
		};
		void run() override;
		bool checkConnection();
		void setColor(osg::Vec3 color);
		void setVibration(const bool b) override;
		hid_device *_controller = nullptr;

		static hid_device_info* allHidDevices;
		static wchar_t* getFreeDeviceSN();
		static void reset();
		static bool enumeratedHidDevices;

	private:
		int getBitAt(int k, unsigned char * buf);
		int getValueFromKey(const PS4KEY type, unsigned char *buffer);
		int calcDecimalFromBinar(unsigned char *binar, int i, int jMax);

		float m_deadzoneX, m_deadzoneY;

		unsigned char m_writeBuffer[32];
		bool m_vibrate = false;
		wchar_t* m_serialNumber = NULL;
	};
}
}