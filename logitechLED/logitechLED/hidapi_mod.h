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

/** @file
 * @defgroup API hidapi API
 */

#ifndef HIDAPI_H__
#define HIDAPI_H__

#include <wchar.h>

/** hidapi info structure */
struct hid_device_info {
	/** Platform-specific device path */
	char *path;
	/** Device Vendor ID */
	unsigned short vendor_id;
	/** Device Product ID */
	unsigned short product_id;
	/** Manufacturer String */
	wchar_t *manufacturer_string;
	/** Product string */
	wchar_t *product_string;

	/** Pointer to the next device */
	struct hid_device_info *next;
};

/** @brief Enumerate the HID Devices.

	This function returns a linked list of all the HID devices
	attached to the system which match vendor_id and product_id.
	If @p vendor_id and @p product_id are both set to 0, then
	all HID devices will be returned.

	@ingroup API
	@param vendor_id The Vendor ID (VID) of the types of device
		to open.
	@param product_id The Product ID (PID) of the types of
		device to open.

	@returns
		This function returns a pointer to a linked list of type
		struct #hid_device, containing information about the HID devices
		attached to the system, or NULL in the case of failure. Free
		this linked list by calling hid_free_enumeration().
*/
struct hid_device_info * hid_enumerate(unsigned short vendor_id, unsigned short product_id);

/** @brief Free an enumeration Linked List

	This function frees a linked list created by hid_enumerate().

	@ingroup API
	@param devs Pointer to a list of struct_device returned from
		    	hid_enumerate().
*/
void hid_free_enumeration(struct hid_device_info *devs);

/** @brief Get The Manufacturer String from a HID device.

	@ingroup API
	@param device A device handle returned from hid_open().
	@param string A wide string buffer to put the data into.
	@param maxlen The length of the buffer in multiples of wchar_t.
*/
#endif
