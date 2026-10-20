/**
 * @file nled.c
 * @brief 数値LEDの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#include "nled.h"

#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

void number_led(int n)
{
#define MOD 10
  static int ledc = 0;
  int j;
  j = ledc / MOD;

  switch (j % 4)
  {
  case 0: // 1桁目の表示
    P4DR = ((n % 10) << 4) | 0x08;
    break;
  case 1: // 2桁目の表示
    P4DR = (((n / 10) % 10) << 4) | 0x04;
    break;
  case 2: // 3桁目の表示
    P4DR = (((n / 100) % 10) << 4) | 0x02;
    break;
  case 3: // 4桁目の表示
    P4DR = (((n / 1000) % 10) << 4) | 0x01;
    break;
  }
  ledc = (ledc + 1) % (4 * MOD);
  return;
}

void number_led_off(void)
{
  P4DR = 0;
  return;
}
