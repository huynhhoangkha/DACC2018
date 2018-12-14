// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       DCCAssignment.ino
    Created:	26/11/2018 9:34:51 CH
    Author:     KHAHUYNH\huynh
*/
#include <Wire.h>
#include "InOut.h"
#include "sensor.h"
#include "SerialReceiver.h"
#include "Authentication.h"


void setup() {
	Serial.begin(57600);
	initLCD();
	setPinMode();
	initOutput();
	connectSensor();
	waitForHWSerialMessage();
	state = _INIT;
}

void loop() {
	if (hwSerialFlag) handleHWSerialMessage();
	finger.getTemplateCount();
	if (finger.templateCount == 0) {
		Serial.println("No fingerprint found, please enroll");
		while (getFingerprintEnroll(0) == -1) delay(10);
		finger.getTemplateCount();
	}
	getAuthentication();
	delay(10);
}
