/**
 * @file led.c
 * @brief LEDの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#include "led.h"

#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

void led(int n)
{ // bit4は使用できない、bit4以外は1で点灯
	static int oled = 0;
	if (oled != n)
	{ // 同じ場合は実行しない。
		oled = n;
		PBDR = ~(((n & 0xf0) << 1) | (n & 0x0f)); // 2022.01.26 mod
	}
	return;
}
