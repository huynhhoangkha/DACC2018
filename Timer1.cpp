#include "Timer1.h"
#include <Arduino.h>
#include <stdint.h>

void timer1Schedule_ms(void(*callback)(void), uint32_t time_ms, bool repeat) {
	if (time_ms == 0) callback();
	if (time_ms <= 0) return;
	timer1_disable();
	timer1_detachInterrupt();
	timer1_attachInterrupt(callback);
	timer1_enable(TIM_DIV256, TIM_EDGE, repeat);
	timer1_write(312 * time_ms);
}