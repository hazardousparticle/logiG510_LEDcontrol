#include "hidapi_mod.h"
#include "Logi510.h"

#include <iostream>
#include <cstdlib>

using namespace std;

#define SPEED 50

HANDLE dev_handle = NULL;
void cleanup(void);

int main(int argc, char* argv[])
{
	//cleanup code:
	atexit(cleanup);

	dev_handle = kb_device_open(LOGI_510_VID, LOGI_510_PID);

	if (NotValidHandle(dev_handle))
	{
		cout << "Error" << endl;
		return 0;
	}

	Color *c1 = getL510_LEDColor(dev_handle);

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	c1->separate(r, g, b);

	delete c1;

	printf_s("Read LED color: red=%02X, green=%02X, blue=%02X", r, g, b);
	FreeConsole();

	Color c = Color(r,g,b);
	HsvColor hsv;
	hsv.s = 0xff;
	hsv.v = 0xff;
	hsv.h = 0;

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
	}

	return 0;
}

void cleanup(void)
{
	kb_device_close(dev_handle);
}
