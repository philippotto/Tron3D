#pragma once
#include "gamepadps4.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
// troen
#include "bikeinputstate.h"


// VID and PID values of the specific device
#define VID		0x054c
#define PID		0x05c4

using namespace troen::input;

GamepadPS4::~GamepadPS4()
{
	hid_close(_controller);
	hid_exit();
}

/*
returns foreach type whether it is pressed (1) or not (0)
But for {LEFT|RIGHT}_HAT_{X|Y} it returns a value between 0 and 255.
Atttion: The positon without intercating of {LEFT|RIGHT}_HAT_{X|Y} differs between 125 and 130
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
		int index = i * jMax + j;
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

/*
print (iEnd - iStart) times 8 bits of buffer
*/
void GamepadPS4::printBinary(unsigned char *buffer, int iStart, int iEnd){
	unsigned long ulBytes = 0x00;
	unsigned long n = ulBytes;
	for (int i = iStart; i<iEnd; i++) {
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


//check for events and handle them
bool GamepadPS4::refresh()
{
	unsigned char buf[64];

	//ckeck whether controller is available, if not search for controller
	if (hid_read(_controller, buf, 64) == -1){
		return false;
	};
	unsigned char *buffer = buf;

	//controlled by LEFT_HAT
	float normLX = (getValueFromKey(GamepadPS4::PS4KEY::LEFT_HAT_X, buffer) - 128) / 128.f;
	float leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
	m_bikeInputState->setAngle(-leftStickX);
	return true;
}

bool GamepadPS4::checkConnection(){
	//initialise hid api 
	hid_init();

	// Open the device using the vendor_id, product_id,
	// and optionally the Serial number.
	if ((_controller = hid_open(VID, PID, NULL)) != NULL)
		return true;

	return false;
}