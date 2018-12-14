#pragma once
#ifndef __SERIAL_RECEIVER__
#define __SERIAL_RECEIVER__
#include <Arduino.h>
#include <Timer1.h>
#include <SoftwareSerial.h>
#include "lcd.h"
#include "Authentication.h"
#include "sensor.h"
extern "C" {
#include "user_interface.h"
}

void waitForHWSerialMessage();
void handleHWSerialMessage();

extern os_timer_t hwSerialTimer;
extern char hwSerialReceiveBuffer[256];
extern volatile bool hwSerialFlag;
extern volatile bool hwserialTimedOut;
extern volatile int hwSerialBufferCounter;

#endif // !__SERIAL_RECEIVER__
