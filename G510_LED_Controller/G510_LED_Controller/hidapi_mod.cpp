/*******************************************************
HIDAPI - Multi-Platform library for
communication with HID devices.

Alan Ott
Signal 11 Software

8/22/2009

Copyright 2009, All Rights Reserved.

At the discretion of the user of this library,
this software may be licensed under the terms of the
GNU Public License v3, a BSD-Style license, or the
original HIDAPI license as outlined in the LICENSE.txt,
LICENSE-gpl3.txt, LICENSE-bsd.txt, and LICENSE-orig.txt
files located at the root of the source distribution.
These files may also be found in the public source
code repository located at:
http://github.com/signal11/hidapi .
********************************************************/

/*Modified version of hidapi*/

#include <Windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <hidpi.h>

#include "hidapi_mod.h"
#include "DeviceOpen.h"

//extern HANDLE open_device_handle(const char *path);

struct hid_device_info * hid_enumerate(unsigned short vendor_id, unsigned short product_id)
{
	struct hid_device_info *root = NULL; // return object
	struct hid_device_info *cur_dev = NULL;

	// Windows objects for interacting with the driver.
	GUID InterfaceClassGuid = { 0x4d1e55b2, 0xf16f, 0x11cf, { 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };
	SP_DEVINFO_DATA devinfo_data;
	SP_DEVICE_INTERFACE_DATA device_interface_data;
	SP_DEVICE_INTERFACE_DETAIL_DATA_A *device_interface_detail_data = NULL;
	HDEVINFO device_info_set = INVALID_HANDLE_VALUE;
	int device_index = 0;

	// Initialize the Windows objects.
	devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
	device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// Get information for all the devices belonging to the HID class.
	device_info_set = SetupDiGetClassDevsA(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	// Iterate over each device in the HID class, looking for the right one.

	while(1)
	{
		BOOL res = TRUE;
		HANDLE write_handle = INVALID_HANDLE_VALUE;
		DWORD required_size = 0;
		HIDD_ATTRIBUTES attrib;

		res = SetupDiEnumDeviceInterfaces(device_info_set,
			NULL,
			&InterfaceClassGuid,
			device_index,
			&device_interface_data);

		if (!res) {
			// A return of FALSE from this function means that
			// there are no more devices.	
			break;
		}

		// Call with 0-sized detail size, and let the function
		// tell us how long the detail struct needs to be. The
		// size is put in &required_size.
		res = SetupDiGetDeviceInterfaceDetailA(device_info_set,
			&device_interface_data,
			NULL,
			0,
			&required_size,
			NULL);


		// Allocate a long enough structure for device_interface_detail_data.
		device_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*)malloc(required_size);
		device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

		// Get the detailed data for this device. The detail data gives us
		// the device path for this device, which is then passed into
		// CreateFile() to get a handle to the device.
		res = SetupDiGetDeviceInterfaceDetailA(device_info_set,
			&device_interface_data,
			device_interface_detail_data,
			required_size,
			NULL,
			NULL);

		//bad device, move to next
		if (!res)
		{
			free(device_interface_detail_data);
			device_index++;
			continue;
		}

		// Open a handle to the device
		write_handle = open_device_handle(device_interface_detail_data->DevicePath);

		// Check validity of write_handle.
		if (write_handle == INVALID_HANDLE_VALUE) {
			// Unable to open the device.
			free(device_interface_detail_data);
			device_index++;

			continue;
		}

		// Get the Vendor ID and Product ID for this device.
		attrib.Size = sizeof(HIDD_ATTRIBUTES);
		HidD_GetAttributes(write_handle, &attrib);

		// Check the VID/PID to see if we should add this
		// device to the enumeration list.
		if (attrib.VendorID == vendor_id && attrib.ProductID == product_id)
		{
			std::string str = "";

			struct hid_device_info *tmp;
			PHIDP_PREPARSED_DATA pp_data = NULL;

			/* VID/PID match. Create the record. */
			tmp = (struct hid_device_info*) calloc(1, sizeof(struct hid_device_info));
			if (cur_dev) {
				cur_dev->next = tmp;
			}
			else {
				root = tmp;
			}
			cur_dev = tmp;

			/* Fill out the record */
			cur_dev->next = NULL;
			str = device_interface_detail_data->DevicePath;
			if (str.length() > 0) {
				cur_dev->path = str;
			}
			else
				cur_dev->path = "";

			/* VID/PID */
			cur_dev->vendor_id = attrib.VendorID;
			cur_dev->product_id = attrib.ProductID;
		}
		
		//move to next device
		free(device_interface_detail_data);
		CloseHandle(write_handle);

		device_index++;
	}

	// Close the device information handle.
	SetupDiDestroyDeviceInfoList(device_info_set);

	return root;
}

void hid_free_enumeration(struct hid_device_info *devs)
{
	struct hid_device_info *d = devs;
	while (d) {
		struct hid_device_info *next = d->next;
		free(d);
		d = next;
	}
}

