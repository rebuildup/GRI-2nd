/**
 * @file timer.h
 * @brief タイマーの制御モジュール(ヘッダー)
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

#define SYSCLOCK 20000000										 ///< クロック周波数[Hz]
#define SYSCLOCK_1000 (SYSCLOCK / 1000)							 ///< 1ミリ秒間のクロック振動数
#define COUNT_100MSEC (SYSCLOCK + (8192 * 10 / 2)) / (8192 * 10) ///< 100[msec]に対応するカウント値

/// タイマーのカウンター値に関するデータ
typedef struct
{
	int count;		  ///経過時間をカウントするための変数
	int onecountusec; /// 1カウントあたりの時間[usec]
} timerv;
typedef timerv *timervp;

///タイマー0のカウンターオブジェクト取得
timervp timer_get_timerv0(void);

/**
@brief タイマー0の初期化

タイマー0の初期化を行います。指定した時間間隔ごとにハンドラーを呼び出し、timervオブジェクトのcount値を増やす。
@param[in] compare ハンドラーの起動間隔（コンペアマッチの値）
@param[in] timer_handler_body 起動されるハンドラー
*/
void init_timer0(int compare, void (*timer_handler_body)(void));

/**
 @brief タイマー0のハンドラー起動間隔の設定

 タイマー0がハンドラーを起動する時間間隔をコンペアマッチ値で指定する。

 @param[in] compare ハンドラーの起動間隔（コンペアマッチの値）
*/
void set_period_timer0(int compare);

///タイマー0をスタートする。
void start_timer0(void);

///タイマー0を停止する。
void stop_timer0(void);
