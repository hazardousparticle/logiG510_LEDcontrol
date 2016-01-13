#include "hidapi_mod.h"
#include "Logi510.h"

#include <iostream>

using namespace std;

//delay in ms between color cycles
#define SPEED 100

//button Q to break the loop
#define QUIT_KEY 0x51

HANDLE dev_handle = NULL;

int main(int argc, char* argv[])
{
	dev_handle = kb_device_open(LOGI_510_VID, LOGI_510_PID);

	if (NotValidHandle(dev_handle))//check if g510 found
	{
		cout << "No G510 keyboard found." << endl;

		//if not check for a g110
		dev_handle = kb_device_open(LOGI_510_VID, LOGI_110_PID);
		if (NotValidHandle(dev_handle))
		{
			cout << "No G110 keyboard found." << endl;

			return 0;
		}
		else
		{
			cout << "TODO: modify LOGI_510_COLOR_CHANGE_CMD according to g110" << endl;
			cout << "G110 device found but currently not supported" << endl;
			return 0;
		}

	}

	Color *c1 = getL510_LEDColor(dev_handle);

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	c1->separate(r, g, b);

	printf_s("Read LED color: red=%02X, green=%02X, blue=%02X\r\n", r, g, b);
	//FreeConsole();

	Color c = Color(r,g,b);

	HsvColor hsv;
	hsv.s = (unsigned char)0xff;
	hsv.v = (unsigned char)0xff;
	hsv.h = (unsigned char)0;

	while (1)
	{
		HsvToRgb(hsv, c);
		setL510_LEDColor(dev_handle, &c);
		hsv.h++;
		if (hsv.h >= 0xff)
		{
			hsv.h = 0;
		}
		Sleep(SPEED);

		//signal to quit
		if (GetAsyncKeyState(QUIT_KEY))
		{
			cout << "Signal to quit..." << endl;
			break;
		}

	}

	setL510_LEDColor(dev_handle, c1);

	delete c1;
	c1 = nullptr;
	
	kb_device_close(dev_handle);
	return 0;
}
