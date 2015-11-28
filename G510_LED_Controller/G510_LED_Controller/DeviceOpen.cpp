#include "DeviceOpen.h"
#include "Stringapiset.h"

//opens devices

HANDLE open_device_handle(std::string path)
{

	if (path == "" || path.length() < 1)
	{
		//bad path to device
		return INVALID_HANDLE_VALUE;
	}

	HANDLE handle;

	/*Some devices must be opened with sharing enabled */
	int sz = path.length() + 1;
	wchar_t * wpath = new wchar_t[sz];

	mbstowcs_s(nullptr, wpath, sz, path.c_str(), _TRUNCATE);

	handle = CreateFile(wpath,	//0,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, /*share mode*/
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL,
		NULL);

	delete[] wpath;

	return handle;
}
