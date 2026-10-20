/* c2pad h8sampleDETAIL */

timerv *timer_get_timerv0(void)
{
	return &t0v;
}

void timer0_handler(void)
{
	t0v.count++;
	タイマーハンドラーの実行;
	など;
}

void init_timer0(int compare, 関数ポインタ)
{
	コンペアマッチをcompareに設定;
	タイマーハンドラーを「関数ポインタ」に設定;
}

void start_timer0(void)
{
	タイマー0の開始;
}

void stop_timer0(void)
{
	タイマー0の停止;
}

void motor_fwd(void)
{
	モーター正転;
}

void motor_rev(void)
{
	モーター逆転;
}

void motor_stop(void)
{
	モーターストップ;
}

int motor_encoder_state(void)
{
	モーターのエンコーダー値を取得し、下位2ビットをB相A相（数値0～3）で返却;
}

/* pad2ps: newpage */

void number_led(int n)
{
	数値LEDの表示制御;
}

void number_led_off(void)
{
	数値LEDの消灯;
}

void led(int n)
{ // bit4は消灯(使用できない)、bit4以外は1で点灯
	LEDのbit4以外を2進数で点灯;
}

/* pad2ps: newpage */

// 初期化
void hardware_init(void)
{
	H8ポートの初期化;
}

int main(void)
{
	各種変数の宣言と初期化;
	argvからの数値読取り;
	hardware_init();
	init_timer0(COUNT_100MSEC, null);
	start_timer0();

	while (1)
	{
		スタートとストップの同時押しでbreak;
		スタートスイッチで motor_on = 1;
		ストップスイッチで motor_on = 0;
		モーター制御();
		モーター回転数();
		number_led(motor_rotation_count);
		点滅();
	}
	終了処理();
	使い方を表示する;
	return 0;
}

/* pad2ps: newpage */

詳細 モーター制御()
{
	説明：motor_onフラグとトグルスイッチでモーターの正転・逆転・停止を制御;
	if (motor_on)
	{
		if (トグルスイッチが正転側)
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
}

詳細 モーター回転数()
{
	説明：初期化でprevious_encoder_state = motor_encoder_state(), エンコーダー状態に応じてmotor_rotation_countを増加・減少;
	current_encoder_state = motor_encoder_state();
	if ((previous_encoder_state == 3) && (current_encoder_state == 1))
	{
		motor_rotation_count++ インクリメント;
		if (motor_rotation_count > 9999)
		{
			説明：9999を超えたら0に戻す;
			motor_rotation_count = 0;
		}
	}
	if ((previous_encoder_state == 3) && (current_encoder_state == 2))
	{
		motor_rotation_count-- デクリメント;
		if (motor_rotation_count < 0)
		{
			説明：マイナスになったら9999にする;
			motor_rotation_count = 9999;
		}
	}
	previous_encoder_state = current_encoder_state;
}

/* pad2ps: newpage */

詳細 点滅()
{
	説明： last_blink_interval = -1, led_pattern = -1;
	if (timer_get_timerv0()->count / 5 !=  last_blink_interval )
	{
		説明：0 .5秒ごとに以下を実行;
		last_blink_interval = t0()->count / 5;
		led_patternの反転; //全点灯⇔全消灯
		led(led_pattern);
	}
}
