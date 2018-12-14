#pragma once
#ifndef _FPM10A_
#define _FPM10A_

#include <Arduino.h>
#include "Adafruit_Fingerprint.h"
#include "lcd.h"
#include "Authentication.h"

void connectSensor();
bool getFingerprintIDez(int& id);
bool getFingerprintID(int& id);
int getFingerprintEnroll(uint8_t id);
uint8_t deleteFingerprint(uint8_t id);

extern Adafruit_Fingerprint finger;

#endif // !_FPM10A_
