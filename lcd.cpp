#include "lcd.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

void display(char* str, int pos, int row = 0) {
	if (row == 1) lcd.setCursor(pos > 0 ? pos : 0, 1);
	int offset = 0;
	if (pos < 0) offset = -pos;
	char* _str = (char*)str + offset;
	int len = strlen(_str);
	for (int i = 0; i < len; i++)
		lcd.write(_str[i]);
}

void initLCD() {
	lcd.init();
	lcd.backlight();
}

void clearLCD() { lcd.clear(); }
