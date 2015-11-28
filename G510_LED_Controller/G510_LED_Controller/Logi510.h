#pragma once
#ifndef _LOGI_510_H_
#define _LOGI_510_H_

//constants
//#define CMD_SIZE 0x102

#define LOGI_510_COLOR_CHANGE_CMD 0x05

#define LOGI_510_VID 0x046d
#define LOGI_510_PID 0xc22d

//includes
#include <Windows.h>
#include <hidclass.h>
#include <Hidsdi.h>
#include "Color.h"


//function prototypes
Color* getL510_LEDColor(HANDLE dev);
void setL510_LEDColor(HANDLE dev, Color *c);
extern bool NotValidHandle(HANDLE h);

HANDLE kb_device_open();
void kb_device_close(HANDLE h);
bool NotValidHandle(HANDLE h);

#endif
