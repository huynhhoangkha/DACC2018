#pragma once
#ifndef _DCC_LCD_
#define _DCC_LCD_
#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#define LCD_I2C_ADDRESS	0x3F

void display(char* str, int pos, int row);
void initLCD();
void clearLCD();

extern LiquidCrystal_I2C lcd;

#endif // !_DCC_LCD_
