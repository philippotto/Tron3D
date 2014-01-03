#include "gamepadps4.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
#include <cmath>

// VID and PID values of the specific device
#define VID		0x054c
#define PID		0x05c4

using namespace troen::input;

GamepadPS4::GamepadPS4(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState)
{
	m_deadzoneX = 0.05f;
	m_deadzoneY = 0.02f;

	//initialise hid api
	hid_init();
}

GamepadPS4::~GamepadPS4()
{
	hid_close(_controller);
	hid_exit();
}

/*
Returns for each type whether it is pressed (1) or not (0).
For {LEFT|RIGHT}_HAT_{X|Y} it returns a value between 0 and 255.
*/
int GamepadPS4::getValueFromKey(GamepadPS4::PS4KEY type, unsigned char *buffer){

	int cross = calcDecimalFromBinar(buffer, 5, 4);
	switch (type){
	case GamepadPS4::PS4KEY::ONE_PRESSED: return getBitAt(44, buffer);
	case GamepadPS4::PS4KEY::TWO_PRESSED: return getBitAt(45, buffer);
	case GamepadPS4::PS4KEY::THREE_PRESSED: return getBitAt(46, buffer);
	case GamepadPS4::PS4KEY::FOUR_PRESSED: return getBitAt(47, buffer);
	case GamepadPS4::PS4KEY::LEFT_1_PRESSED: return getBitAt(48, buffer);
	case GamepadPS4::PS4KEY::RIGHT_1_PRESSED: return getBitAt(49, buffer);
	case GamepadPS4::PS4KEY::LEFT_2_PRESSED: return getBitAt(50, buffer);
	case GamepadPS4::PS4KEY::RIGHT_2_PRESSED: return getBitAt(51, buffer);
	case GamepadPS4::PS4KEY::SHARE_PRESSED: return getBitAt(52, buffer);
	case GamepadPS4::PS4KEY::OPTION_PRESSED: return getBitAt(53, buffer);
	case GamepadPS4::PS4KEY::LEFT_HAT_PRESSED: return getBitAt(54, buffer);
	case GamepadPS4::PS4KEY::RIGHT_HAT_PRESSED: return getBitAt(55, buffer);
	case GamepadPS4::PS4KEY::LEFT_HAT_X: return calcDecimalFromBinar(buffer, 1, 8);
	case GamepadPS4::PS4KEY::LEFT_HAT_Y: return calcDecimalFromBinar(buffer, 2, 8);
	case GamepadPS4::PS4KEY::RIGHT_HAT_X: return calcDecimalFromBinar(buffer, 3, 8);
	case GamepadPS4::PS4KEY::RIGHT_HAT_Y: return calcDecimalFromBinar(buffer, 4, 8);

	case GamepadPS4::PS4KEY::UP_PRESSED: if (cross == 7 || cross == 0 || cross == 1) return 1;
										   else return 0;
	case GamepadPS4::PS4KEY::RIGHT_PRESSED: if (cross == 1 || cross == 2 || cross == 3) return 1;
											  else return 0;
	case GamepadPS4::PS4KEY::DOWN_PRESSED: if (cross == 3 || cross == 4 || cross == 5) return 1;
											 else return 0;
	case GamepadPS4::PS4KEY::LEFT_PRESSED: if (cross == 5 || cross == 6 || cross == 7) return 1;
											 else return 0;
	case GamepadPS4::PS4KEY::LEFT_2: return calcDecimalFromBinar(buffer, 8, 8);
	case GamepadPS4::PS4KEY::RIGHT_2: return calcDecimalFromBinar(buffer, 9, 8);
	default: return -1;
	}
}

/*
return reads jMax bits at i*8 and interprete this as decimal
*/
int GamepadPS4::calcDecimalFromBinar(unsigned char *binar, int i, int jMax){
	long ulBytes = 0x00;
	int tmp = 0;
	long n = ulBytes | binar[i];
	for (int j = 0; j < jMax; j++)
	{
		//int index = i * jMax + j;
		if (n & 1)
			tmp += pow(2, j);

		n >>= 1;
	}
	return tmp;
}

/*
returns buffer's bit at k
*/
int GamepadPS4::getBitAt(int k, unsigned char * buffer){
	long ulBytes = 0x00;
	for (int i = 0; i<8; i++) {
		long n = ulBytes | buffer[i];
		for (int j = 0; j < 8; j++) {
			if (i * 8 + j == k){
				if (n & 1)
					return 1;
				else
					return 0;
			}
			n >>= 1;
		}
	}
	return -1;
}

//check for events and handle them
void GamepadPS4::run()
{
	m_pollingEnabled = true;

	while (m_pollingEnabled)
	{
		unsigned char buf[96];
		// check whether controller is available, if not search for controller
		// if it is still not available do nothing
		if (!_controller || hid_read(_controller, buf, 96) == -1){
			if (!checkConnection())
			{
				m_bikeInputState->setAngle(0);
				m_bikeInputState->setAcceleration(0);
				return;
			};
		}

		// offset, as the first 16 bytes are garbage
		unsigned char *buffer = &buf[2];

		// get angle value from LEFT_HAT
		float normLX = (getValueFromKey(GamepadPS4::PS4KEY::LEFT_HAT_X, buffer) - 128) / 128.f;
		float leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
		m_bikeInputState->setAngle(-leftStickX);

		// get acceleration from RIGHT_2 and LEFT_2
		float rightTrigger = getValueFromKey(GamepadPS4::PS4KEY::RIGHT_2, buffer) / 255.f;
		float leftTrigger = getValueFromKey(GamepadPS4::PS4KEY::LEFT_2, buffer) / 255.f;
		m_bikeInputState->setAcceleration(rightTrigger - leftTrigger);

		this->msleep(POLLING_DELAY_MS);
	}
}

bool GamepadPS4::checkConnection(){
	// Open the device using the vendor_id, product_id,
	// and optionally the Serial number.
	if ((_controller = hid_open(VID, PID, nullptr)) != nullptr)
		return true;

	return false;
}