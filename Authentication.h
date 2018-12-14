#pragma once
#ifndef __DCC_AUTHENTICATION_H_
#define __DCC_AUTHENTICATION_H_

#include <Arduino.h>
#include "Timer1.h"
#include "sensor.h"
#include "lcd.h"
#include "InOut.h"


enum dfaState {
	_INIT,
	_NO_FINGERPRINT,
	_INVALID_FINGERPRINT,
	_VALID_FINGERPRINT
};

void ICACHE_RAM_ATTR validFingerPrintTimeoutISR();
void ICACHE_RAM_ATTR invalidFingerPrintISR();
int getAuthentication();
void disableTimeout();
void enableTimeout();

extern volatile dfaState state;
extern volatile bool authenticated;
extern volatile bool timeoutEnabled;

#endif // !__DCC_AUTHENTICATION_H_
