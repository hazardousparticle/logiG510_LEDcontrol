#include "Logi510.h"

//set color: write the color information in the given color object to the device.
void setL510_LEDColor(HANDLE dev, Color *c)
{
	if (NotValidHandle(dev))
	{
		return;
	}
	unsigned char cmd[4] = { 0 };
	cmd[0] = LOGI_510_COLOR_CHANGE_CMD;
	c->separate(cmd[1], cmd[2], cmd[3]);

	if (!HidD_SetFeature(dev, cmd, sizeof(cmd)))
	{
		//error failed
	}
}


//get color: returns a pointer to a color object containing the color read from the device.
Color* getL510_LEDColor(HANDLE dev)
{
	if (NotValidHandle(dev))
	{
		return new Color();
	}

	unsigned char reply[4] = { 0 };
	reply[0] = LOGI_510_COLOR_CHANGE_CMD;

	if (!HidD_GetFeature(dev, reply, sizeof(reply)))
	{
		//error occured
		return new Color();
	}

	Color *c_new = new Color(reply[1], reply[2], reply[3]);
	return c_new;
}
