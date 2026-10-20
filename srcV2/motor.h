/**
 * @file motor.h
 * @brief モーターおよびエンコーダーの制御モジュール（ヘッダー）
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

/// モーターを正転する。
void motor_fwd(void);

/// モーターを逆転する。
void motor_rev(void);

/// モーターをストップする（こぐのをやめる）。
void motor_stop(void);

/// エンコーダの状態を下位2ビット[B相 A相]（数値0～3）で返却する。
int motor_encoder_state(void);
