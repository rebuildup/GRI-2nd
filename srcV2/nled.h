/**
 * @file nled.h
 * @brief 数値LEDの制御モジュール(ヘッダー)
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

/**
 * @brief 数値LED点灯関数
 * @param [in] n 表示する数値
 * @details
 * 詳細説明
 * - 数値LED点灯関数
 * - MOD によりダイナミック表示切替え時間を制御
 */
void number_led(int n);

/// 7セグの表示オフ
void number_led_off(void);
