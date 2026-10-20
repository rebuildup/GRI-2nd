/**
 * @file led.c
 * @brief LEDの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#include "led.h"
#include "timer.h"

#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

typedef struct
{
	int pattern;
	int period;
	int mode;

} ledv;
typedef ledv *ledvp;
static ledv _ledValue;

ledvp lv(void)
{
	return &_ledValue;
}
void ledout(int n)
{
	// bit4は使用できない、bit4以外は1で点灯
	static int oled = 0;
	if (oled != n)
	{ // 同じ場合は実行しない。
		oled = n;
		PBDR = ~(((n & 0xf0) << 1) | (n & 0x0f)); // 2022.01.26 mod
	}
	return;
}

void led(int pat)
{
	_ledValue.pattern = pat;
	return;
}

void ledblinkmode(int mode) // led_blink_mode
{
	_ledValue.mode = mode;
	return;
}

void ledblinktime(int period) // led_blink_period
{
	_ledValue.period = period;
	return;
}

void ledexec()
{
	int count = timer_get_timerv0()->count;
	int period = _ledValue.period;
	int mode = _ledValue.mode;
	int is_blinking = 0; // false
	switch (mode)
	{
	case 0:
		is_blinking = 1; // 点灯
		break;
	case 1:
		if (count / (period / 200) % 2 == 0)
			is_blinking = 1;
		break;
	case 2:
		if (count / ((period + 100) / 200) % 2 == 0)
			is_blinking = 1;
		break;
	case 3:
		if (((count * 200) / period) % 2 == 0)
			is_blinking = 1;
		break;
	default:
		is_blinking = 1;
		break;
	}
	if (is_blinking)
		ledout(_ledValue.pattern);
	else
		ledout(0);
}
