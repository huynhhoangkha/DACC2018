#pragma once
#ifndef _ESP8266_TIMER1_
#define _ESP8266_TIMER1_

#include <Arduino.h>
#include <stdint.h>

void timer1Schedule_ms(void(*callback)(void), uint32_t time_ms, bool repeat);

#endif // !_ESP8266_TIMER1_

