/**
 * @file hardware_init.c
 * @brief ハードウェアの初期化モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#include "hardware_init.h"
void hardware_init(void) {
  //モーターとエンコーダー
  //ポートAの0〜1をエンコーダーからの入力に、2〜7を（モーターへの）出力用に設定
  PADDR = 0xfc; /* o_o */
  // LED
  PBDDR = 0xff; //ポートBを出力用に設定（0〜7）
  PBDR = 0xff;  //全LED消灯
  //数値LED
  P4DDR = 0xff; //ポート4を出力用に設定（0〜7）
  //スイッチ
  P5DDR = 0xf0; //ポート5を入力用
}


