/**
 * @file led.h
 * @brief LEDの制御モジュール(ヘッダー)
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

/**
 * @brief LED点灯関数
 * @param [in] n 2進表示する数値
 * @details
 * 詳細説明
 * - LED点灯関数
 * - bit4が使用できないので、
 * - bit4を除いた7bitで2進数表示
 */
void led(int n);
