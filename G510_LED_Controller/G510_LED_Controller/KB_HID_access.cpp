#include <windows.h>
#include "hidapi_mod.h"
#include "Logi510.h"
#include "DeviceOpen.h"

#include <iostream>

//this will return a windows device handle of the vid and pid given.
HANDLE kb_device_open(unsigned short vendor_id, unsigned short product_id)
{
	//const unsigned short vendor_id = LOGI_510_VID;
	//const unsigned short product_id = LOGI_510_PID;
	
	if (!vendor_id || !product_id)
	{
		return INVALID_HANDLE_VALUE;
	}

	struct hid_device_info *devs, *cur_dev;
	std::string path_to_open = "";
	HANDLE handle = INVALID_HANDLE_VALUE;

	devs = hid_enumerate(vendor_id, product_id);
	cur_dev = devs;
	while (cur_dev) {

		//ensure the selected device is the last one found.
		if (cur_dev->vendor_id == vendor_id && 
			cur_dev->product_id == product_id)
		{
				path_to_open = cur_dev->path;
				//break;
		}
		cur_dev = cur_dev->next;
	}

	if (path_to_open.length() > 0) {
		/* Open the device */
		handle = open_device_handle(path_to_open);
		std::cout << "Found device: " << path_to_open << std::endl;
	}

	hid_free_enumeration(devs);

	return handle;

	//always check the returned value for a valid handle.
	//returns NULL if device could not be connected.
}

//close the keyboard device
void kb_device_close(HANDLE h)
{
	if (NotValidHandle(h))
	{
		return;
	}

	CancelIo(h);

	CloseHandle(h);

	std::cout << "Device stopped." << std::endl;
}

//check if device handles are ok
bool NotValidHandle(HANDLE h)
{
	return !(h != INVALID_HANDLE_VALUE && h != NULL);
}
