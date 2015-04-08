#include <windows.h>
#include "hidapi_mod.h"
#include "Logi510.h"

//this is a modified version of hidapi's hid_open
//the modified version will return a windows device handle instead of its own.
HANDLE kb_device_open(unsigned short vendor_id, unsigned short product_id)
{
	if (!vendor_id || !product_id)
	{
		return INVALID_HANDLE_VALUE;
	}

	// TODO: Merge this functions with the Linux version. This function should be platform independent.
	struct hid_device_info *devs, *cur_dev;
	const char* path_to_open = NULL;
	HANDLE handle = INVALID_HANDLE_VALUE;

	devs = hid_enumerate(vendor_id, product_id);
	cur_dev = devs;
	while (cur_dev) {
		if (cur_dev->vendor_id == vendor_id && 
			cur_dev->product_id == product_id)
		{
				path_to_open = cur_dev->path;
				//break;
		}
		cur_dev = cur_dev->next;
	}

	if (sizeof(path_to_open) > 0) {
		/* Open the device */
		handle = open_device_handle(path_to_open);
	}

	hid_free_enumeration(devs);

	return handle;
}

void kb_device_close(HANDLE h)
{
	if (NotValidHandle(h))
	{
		return;
	}

	CancelIo(h);

	CloseHandle(h);
}

#include "Stringapiset.h"

//had to copy this from hid.c from hidapi
//calls to this function fail due to linkage of a c file
HANDLE open_device_handle(const char* path)
{
	
	if (!path)
	{
		return INVALID_HANDLE_VALUE;
	}
	if (strlen(path) <= 1)
	{
		return INVALID_HANDLE_VALUE;
	}
	
	HANDLE handle;

	/*Some devices must be opened
		with sharing enabled (even though they are only opened once),
		so try it here. */
	int sz = strlen(path) + 1;
	wchar_t * wpath = new wchar_t[sz];

	mbstowcs_s(nullptr, wpath, sz, path, _TRUNCATE);

	handle = CreateFile(wpath,	//0,
		GENERIC_WRITE| GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, /*share mode*/
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL,
			NULL);

	delete [] wpath;

	return handle;
}

bool NotValidHandle(HANDLE h)
{
	return !(h != INVALID_HANDLE_VALUE && h != NULL);
}
