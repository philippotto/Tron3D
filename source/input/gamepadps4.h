#pragma once
// scriptzeug
#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"

#include <hidapi.h>

namespace troen
{
namespace input
{
	class GamepadPS4 : public PollingDevice, public reflectionzeug::Object
	{
	public:
		GamepadPS4(osg::ref_ptr<BikeInputState> bikeInputState);
		~GamepadPS4();
		enum PS4KEY {
			UP_PRESSED, DOWN_PRESSED, LEFT_PRESSED, RIGHT_PRESSED, ONE_PRESSED, TWO_PRESSED, THREE_PRESSED, FOUR_PRESSED, LEFT_1_PRESSED,
			LEFT_2_PRESSED, RIGHT_1_PRESSED, RIGHT_2_PRESSED, SHARE_PRESSED, OPTION_PRESSED, LEFT_HAT_PRESSED, RIGHT_HAT_PRESSED, LEFT_HAT_X,
			LEFT_HAT_Y, RIGHT_HAT_Y, RIGHT_HAT_X, LEFT_2, RIGHT_2
		};
		void run() override;
		bool checkConnection();
		hid_device *_controller = nullptr;

		void setBuffer(const std::vector<reflectionzeug::Variant> &args);

	private:
		int getBitAt(int k, unsigned char * buf);
		int getValueFromKey(const PS4KEY type, unsigned char *buffer);
		int calcDecimalFromBinar(unsigned char *binar, int i, int jMax);

		float m_deadzoneX, m_deadzoneY;
	};
}
}