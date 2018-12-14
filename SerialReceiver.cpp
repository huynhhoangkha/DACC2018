#include "SerialReceiver.h"

os_timer_t hwSerialTimer;
char hwSerialReceiveBuffer[256];
volatile bool hwSerialFlag = false;
volatile bool hwSerialTimedOut = false;
volatile int hwSerialBufferCounter = 0;

void ICACHE_RAM_ATTR hwTimerISR(void* param) {
	while (Serial.available()) {
		hwSerialTimedOut = false;
		char c = Serial.read();
		if (c < 127 && c > 31) hwSerialReceiveBuffer[hwSerialBufferCounter++] = c;
	}
	if (hwSerialTimedOut) {
		os_timer_disarm(&hwSerialTimer);
		hwSerialFlag = true;
		hwSerialReceiveBuffer[hwSerialBufferCounter++] = '\0';
		return;
	}
	if (hwSerialBufferCounter > 0) hwSerialTimedOut = true;
}

void waitForHWSerialMessage() {
	memset((void*)hwSerialReceiveBuffer, 0, 256);
	hwSerialBufferCounter = 0;
	hwSerialFlag = false;
	hwSerialTimedOut = false;
	Serial.flush();
	os_timer_setfn(&hwSerialTimer, hwTimerISR, NULL);
	os_timer_arm(&hwSerialTimer, 10, true);
}

void handleHWSerialMessage() {
	hwSerialFlag = false;
	Serial.print("Serial recieved: ");
	Serial.println(hwSerialReceiveBuffer);
	if (!authenticated){
		clearLCD();
		display("Authenticate to", 0, 0);
		display("enter admin mode", 0, 1);
		Serial.println("Please authenticate to enter admin mode.");
		state = _NO_FINGERPRINT;
		disableTimeout();
		long t = millis();
		int id = -1;
		while (millis() - t < 5000 && id == -1) {
			id = getAuthentication();
			delay(10);
		}
		if (id < 0) {
			enableTimeout();
			waitForHWSerialMessage();
			return;
		}
	}
	if (strcmp(hwSerialReceiveBuffer, "count") == 0) {
		finger.getTemplateCount();
		Serial.print("Number of fingerprints: ");
		Serial.println(finger.templateCount);
	}
	else if (strcmp(hwSerialReceiveBuffer, "enr") == 0) {
		Serial.println("Entered enrollment mode.");
		finger.getTemplateCount();
		getFingerprintEnroll(finger.templateCount);
	}
	else if (strcmp(hwSerialReceiveBuffer, "del") == 0) {
		clearLCD();
		display("Touch to remove", 0, 0);
		display("your fingerprint", 0, 1);
		Serial.println("Touch to delete your fingerprint");
		state = _NO_FINGERPRINT;
		disableTimeout();
		long t = millis();
		int id = -1;
		while (millis() - t < 5000 && id == -1) {
			id = getAuthentication();
			delay(10);
		}
		if (id == -1) {
			enableTimeout();
			waitForHWSerialMessage();
			return;
		}
		Serial.print("Deleting fingerprint #");
		Serial.println(id);
		deleteFingerprint(id);
		display("Deleted.", 0, 1);
	}
	enableTimeout();
	waitForHWSerialMessage();
	return;
}
