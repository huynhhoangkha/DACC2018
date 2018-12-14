#include "Authentication.h"

volatile dfaState state;
volatile bool authenticated = false;
volatile bool timeoutEnabled = true;

void ICACHE_RAM_ATTR validFingerPrintTimeoutISR() {
	digitalWrite(LED_BUILTIN, HIGH);
	state = _NO_FINGERPRINT;
	digitalWrite(CTLR, LOW);
	Serial.println("Session ended.");
	Serial.println("Scan your finger to access.");
	clearLCD();
	display("Hello, scan your", 0, 0);
	display("finger to access", 0, 1);
	beep(200);
	authenticated = false;
}

void ICACHE_RAM_ATTR invalidFingerPrintISR() {
	state = _NO_FINGERPRINT;
	clearLCD();
	display("Hello, scan your", 0, 0);
	display("finger to access", 0, 1);
	authenticated = false;
}

int getAuthentication() {
	int id = -1;
	switch (state)
	{
	case _INIT:
		clearLCD();
		display("Hello, scan your", 0, 0);
		display("finger to access", 0, 1);
		state = _NO_FINGERPRINT;
		break;
	case _NO_FINGERPRINT:
		if (getFingerprintID(id)) {
			state = _VALID_FINGERPRINT;
			digitalWrite(LED_BUILTIN, LOW);
			if (timeoutEnabled) timer1Schedule_ms(validFingerPrintTimeoutISR, 3000, false);
			clearLCD();
			display("Access granted.", 0, 0);
			authenticated = true;
			digitalWrite(CTLR, authenticated);
		}
		else if (id == -2) {
			state = _INVALID_FINGERPRINT;
			timer1Schedule_ms(invalidFingerPrintISR, 1500, false);
			clearLCD();
			display("Access denied.", 0, 0);
		}
		break;
	case _INVALID_FINGERPRINT:
	case _VALID_FINGERPRINT:
	default:
		// Do nothing.
		break;
	}
	return id;
}

void disableTimeout() {
	timeoutEnabled = false;
}

void enableTimeout() {
	timer1Schedule_ms(validFingerPrintTimeoutISR, 3000, false);
	timeoutEnabled = true;
}
