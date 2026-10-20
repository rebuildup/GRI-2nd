/**
 * @file timer.c
 * @brief タイマーの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

#include "timer.h"

static timerv t0v;

timervp timer_get_timerv0(void)
{
	return &t0v;
}

static void (*timer0_handler_body)(void);

#pragma interrupt //割り込みで呼び出される関数であることを宣言
static void timer0_handler()
{
	load_segment(36);
	t0v.count++;
	if (timer0_handler_body != NULL)
	{
		(*timer0_handler_body)();
	}
	TCSR0_8 &= ~0x40; //第6ビットを0にする。
	return;
}

void init_timer0(int compare, void (*timer_handler_body)(void))
{
	timer0_handler_body = timer_handler_body;
	t0v.count = 0;
	t0v.onecountusec = (int)(((long)compare * 8192 * 1000 + SYSCLOCK_1000 / 2) / SYSCLOCK_1000);
	set_handler(36, timer0_handler);
	TCR0_8 = 0x48;
	TCORA0_8 = compare - 1; //コンペアマッチを設定
	TCSR0_8 = 0x0;			// 出力なし
	TCNT0_8 = 0;
	return;
}

void set_period_timer0(int compare)
{
	t0v.onecountusec = (int)(((long)compare * 8192 * 1000 + SYSCLOCK_1000 / 2) / SYSCLOCK_1000);
	TCORA0_8 = compare - 1; //コンペアマッチを設定
}

void start_timer0(void)
{
	TCR0_8 = 0x4b; // 01 0 01 011 ( 内部クロックψ/8192 立ち上がりエッジでカウント)
	return;
}

void stop_timer0(void)
{
	TCR0_8 = 0;
	return;
}
