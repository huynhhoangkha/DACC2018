#include "InOut.h"
#include "Timer1.h"

os_timer_t beepTimer;

void setPinMode() {
	pinMode(CTLR, OUTPUT);
	pinMode(BUZZ, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
}

void initOutput() {
	digitalWrite(CTLR, LOW);
	digitalWrite(BUZZ, LOW);
	digitalWrite(LED_BUILTIN, HIGH);
}

void beep(int ms) {
	digitalWrite(BUZZ, HIGH);
	os_timer_setfn(&beepTimer, [](void*) {
		digitalWrite(BUZZ, !digitalRead(BUZZ));
	}, NULL);
	os_timer_arm(&beepTimer, ms, false);
}

void testOutput() {
	digitalWrite(BUZZ, HIGH);
	digitalWrite(CTLR, HIGH);
	timer1Schedule_ms([]() ICACHE_RAM_ATTR{
		digitalWrite(BUZZ, !digitalRead(BUZZ));
		digitalWrite(CTLR, !digitalRead(CTLR));
	}, 300, false);
}
