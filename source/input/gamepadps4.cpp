#include "gamepadps4.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
#include "../globals.h"
#include <cmath>

// VID and PID values of the specific device
#define VID		0x054c
#define PID		0x05c4

using namespace troen::input;

void printBinary(unsigned char *buffer, int iStart, int iEnd){
	unsigned long ulBytes = 0x00;
	unsigned long n = ulBytes;
	for (int i = iStart; i < iEnd; i++) {
		long n = ulBytes | buffer[i];
		for (int j = 0; j < 8; j++) {
			if (n & 1)
				printf("1");
			else
				printf("0");
			n >>= 1;
		}
		printf(" ");
	}
	printf("\n");
}


GamepadPS4::GamepadPS4(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState)
{
	m_deadzoneX = 0.05f;
	m_deadzoneY = 0.02f;

	//initialise hid api
	hid_init();

	_controller = hid_open(VID, PID, nullptr);
	unsigned char* b = (unsigned char*)malloc(1000000);
	int start, end;
	/*start = 1;
	end = 1000;*/
	start = 32;
	end = 33;

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "starting................." << std::endl;

	b[0] = 5;
	b[1] = 111;
	b[2] = 247;
	b[3] = 188;
	b[4] = 241;
	b[5] = 115;
	b[6] = 95;
	b[7] = 134;
	b[8] = 152;
	b[9] = 117;
	b[10] = 169;
	b[11] = 64;
	b[12] = 235;
	b[13] = 127;
	b[14] = 21;
	b[15] = 97;
	b[16] = 108;
	b[17] = 11;
	b[18] = 3;
	b[19] = 106;
	b[20] = 25;
	b[21] = 146;
	b[22] = 185;
	b[23] = 12;
	b[24] = 205;
	b[25] = 45;
	b[26] = 29;
	b[27] = 46;
	b[28] = 83;
	b[29] = 137;
	b[30] = 248;
	b[31] = 121;

	// probably not important
	// b[32] = (int)0;

	// toggles (?) first (?) motor
	// 255 always vibrate
	// modifying 4. byte lowers velocity
	b[1] = 255;

	// use 2, 3, 4 to adjust strength?
	
	for (int i = 0; i < 6; i++)
	{
		if (i == 1)
			continue;
		int oldValue = b[i];
		
		b[i] = 0;
		int returnV = hid_write(_controller, b, 32);
		if (returnV != -1)
			std::cout << " you may change byte at index: " << i << std::endl;

		sleep(1);
		b[i] = oldValue;
	}

	std::cout << "now b[1] is 0" << std::endl;


	// at 4 is no vibration (when setting byte 4 to 128 there is no vibration?)
	// at 5 is no vibration (when setting byte 5 to 0)

	b[1] = 0;

	// use 2, 3, 4 to adjust strength?

	for (int i = 0; i < 6; i++)
	{
		if (i == 1)
			continue;
		int oldValue = b[i];

		b[i] = 0;
		int returnV = hid_write(_controller, b, 32);
		if (returnV != -1)
			std::cout << " you may change byte at index: " << i << std::endl;

		sleep(1);
		b[i] = oldValue;
	}
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
				m_bikeInputState->setTurboPressed(false);
				return;
			};
		}

		unsigned char *buffer = buf;

		// get angle value from LEFT_HAT
		float normLX = (getValueFromKey(GamepadPS4::PS4KEY::LEFT_HAT_X, buffer) - 128) / 128.f;
		float leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
		float handbrakePressed = getValueFromKey(GamepadPS4::PS4KEY::ONE_PRESSED, buffer);
		float turboPressed = getValueFromKey(GamepadPS4::PS4KEY::TWO_PRESSED, buffer);

		m_bikeInputState->setAngle(-leftStickX - leftStickX * handbrakePressed * BIKE_HANDBRAKE_FACTOR);
		m_bikeInputState->setTurboPressed(turboPressed);

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
	//if ((_controller = hid_open(VID, PID, nullptr)) != nullptr)
	//	return true;

	//return false;
	return true;
}