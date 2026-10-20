/**
 * @file motor.c
 * @brief モーターおよびエンコーダーの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */

#include "motor.h"

#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

typedef struct
{
	int cnt;   //モーターを制御する変数
	int mflag; // PWM
} motorc;
typedef motorc *motorcp;

static motorc mcv;

static motorcp mc(void)
{
	return &mcv;
}

// モーターを正転する
void motor_fwd(void)
{
	if (mcv.cnt != 0x18)
	{ // 同じ場合は実行しない。
		mcv.mflag = (mcv.mflag & ~3) | 1;
		mcv.cnt = 0x18;
		PADR = mcv.cnt;
	}
	return;
}
// モーターを逆転する
void motor_rev(void)
{
	if (mcv.cnt != 0x14)
	{ // 同じ場合は実行しない。
		mcv.mflag = mcv.mflag | 3;
		mcv.cnt = 0x14;
		PADR = mcv.cnt;
	}
	return;
}
// モーターをストップする（こぐのをやめる）PWMはこちらを使用すること
void motor_stop(void)
{
	if (mcv.cnt != 0x10)
	{ // 同じ場合は実行しない。
		mcv.mflag = (mcv.mflag & ~1);
		mcv.cnt = 0x10;
		PADR = mcv.cnt;
	}
	return;
}

// エンコーダー状態
int motor_encoder_state(void)
{
	// baの値で、 00:0 01:1 10:2 11:3
	return (PADR & 0x03);
}
