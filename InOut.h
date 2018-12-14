#pragma once
#ifndef _IN_OUT_
#define _IN_OUT_

#define CTLR			D8
#define BUZZ			D7
#define FP_TX			D6
#define FP_RX			D5
#define LCD_SDA			D2
#define LCD_SCL			D1
#define SENSOR_BAUDRATE	57600

#include <Arduino.h>
extern "C" {
#include "user_interface.h"
}


void beep(int ms);
void setPinMode();
void initOutput();
void testOutput();

#endif // !_IN_OUT_

