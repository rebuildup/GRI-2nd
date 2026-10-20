/**
 * @file h8sample.c
 * @brief h8モーターを回転するプログラム
 * @author Yuji KATSUTA, Makoto TANABE
 * @date 2025.4.23
 */
#include "getint.h"
#include "timer.h"
#include "motor.h"
#include "led.h"
#include "nled.h"
#include "hardware_init.h"

int main(int argc, char **argv)
{
  // モーターとエンコーダーに関する変数
  int current_encoder_state;    // 現在のエンコーダーの状態
  int previous_encoder_state;   // 前回のエンコーダーの状態
  int motor_rotation_count = 0; // モーターの回転数
  int motor_on = 0;             // モーターの回転有無（0:停止、1:回転）
  // LEDに関する変数

  //===ここから===
  // int led_pattern = -1;        // LEDの点灯パターン（初期値：全点灯）
  int led_pattern = 0x05; // xxxxxoxo

  // LEDの点灯パターン
  int blink_mode = 1;
  //===ここまで

  int led_blink_period = 1000; // LEDの点滅周期（初期値：1000ms）
  // タイマーに関する変数
  int last_blink_interval = 0; // 0.5秒ごとにLEDを点滅させるためのカウンタ

  // デバッグモードに関する変数（オプション引数「-DEBUG」を読み込む）
  int debug = 0; // 初期値：デバッグモード無効

  // 引数の解析

  //===ここから===
  if (getint(argc - 1, argv + 1, "-N", &led_pattern) == 0)
  {
    printf(" N=%d ", led_pattern);
    printf(LF);
  }
  if (getint(argc - 1, argv + 1, "-F", &blink_mode) == 0)
  {
    printf(" F=%d ", blink_mode);
    printf(LF);
  }
  //===ここまで===

  if (getint(argc - 1, argv + 1, "-B", &led_blink_period) == 0)
  {
    printf(" B=%d ", led_blink_period);
    printf(LF);
  }
  if (getint(argc - 1, argv + 1, "-DEBUG", &debug) == 0 && debug)
  { // オプション引数「-DEBUG」として0以外の値が指定された
    printf("DEBUG MODE");
    printf(LF);
  }
  hardware_init();

  //===ここから===
  ledblinkmode(blink_mode);
  ledblinktime(led_blink_period);
  //===ここまで===

  init_timer0(COUNT_100MSEC, NULL);
  start_timer0();
  previous_encoder_state = motor_encoder_state();

  while (1)
  {
    if ((P5DR & 3) == 0)
    {
      break;
    } // スイッチが押されたら終了
    if ((P5DR & 1) == 0)
    {
      motor_on = 1;
    } // モーターON
    if ((P5DR & 2) == 0)
    {
      motor_on = 0;
    } // モーターOFF
    if (motor_on)
    {
      if ((P5DR & 8) == 0)
      {
        motor_fwd();
      }
      else
      {
        motor_rev();
      }
    }
    else
    {
      motor_stop();
    }
    current_encoder_state = motor_encoder_state();
    if ((previous_encoder_state == 3) && (current_encoder_state == 1))
    {
      motor_rotation_count++;
      if (motor_rotation_count > 9999)
      {
        motor_rotation_count = 0;
      }
    }
    if ((previous_encoder_state == 3) && (current_encoder_state == 2))
    {
      motor_rotation_count--;
      if (motor_rotation_count < 0)
      {
        motor_rotation_count = 9999;
      }
    }
    previous_encoder_state = current_encoder_state;
    number_led(motor_rotation_count);

    //===ここから===
    led(led_pattern);
    ledexec();
    /*
    if ((timer_get_timerv0()->count / 5) != last_blink_interval)
    {
      last_blink_interval = timer_get_timerv0()->count / 5;
      led_pattern = ~led_pattern;
      led(led_pattern);
    }
    */
    //===ここまで===

    if (debug)
    { // デバッグモードが有効のときはmcount値を標準出力する。
      printf("motor_rotation_count = %d", motor_rotation_count);
      printf(LF);
    }
  }
  motor_stop();
  stop_timer0();
  led(0);
  number_led_off();
  printf(" usage: %s [-Bxxx] [-Fy] [-Nzz] [-DEBUG1]" LF
         "   xxx: LED blink cycle [msec]" LF,
         "   y: 1:blink、0:nonblink" LF,
         "   zz: bit pattern" LF,
         "   -DEBUG1: turns on debug mode" LF,
         argv[0]);
  return 0;
}
