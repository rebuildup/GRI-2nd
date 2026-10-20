/* c2pad h8sampleledDETAIL */

Ver(1.20)
{
	"2025/4/23";
}

ledv *lv(void)
{
	return &_ledValue;
}

void ledout(int n)
{ // bit4は消灯(使用できない)、bit4以外は1で点灯
	説明：LEDのbit4以外を2進数で点灯;
	static int oled;
	if (oled != n)
	{
		oled = n;
		PBDR = ~n | 0x10;
	}
}

void led(int n)
{
	説明：pattern の設定;
	_ledValue.pattern = n;
}

/* pad2ps: newpage */

void ledblinkmode(int mode)
{
	説明：点滅モード(mode)の設定;
	_ledValue.mode = mode;
}

void ledblinktime(int msec)
{
	説明：点滅周期(period)の設定;
	_ledValue.period = msec;
}

void ledexec(void)
{
	int count = timer_get_timerv0()->count;
	int period = _ledValue.period;
	int mode = _ledValue.mode;
	int blink_phase;

#define MIN_BLINK_PERIOD_MS 200

	switch (mode)
	{
	case 0:
		blink_phase = 0;
		break;

	case 1:
		方法1「点滅周期を200で割り切れる最小単位に切り捨て」:
		blink_phase = count / (period / MIN_BLINK_PERIOD_MS);
		break;

	case 2:
		方法2「点滅周期を200に四捨五入して近似」:
		blink_phase = count / ((period + MIN_BLINK_PERIOD_MS / 2) / MIN_BLINK_PERIOD_MS);
		break;

	case 3:
		方法3「カウント値と点滅周期に応じて、長期的に正確な周期になるように点滅を判断」:
		blink_phase = count * MIN_BLINK_PERIOD_MS / period;
		break;
	}

	if (blink_phase % 2 == 0)
	{
		ledout(_ledValue.pattern); // 点灯
	}
	else
	{
		ledout(0); // 消灯
	}
}

int main(int argc, char **argv)
{
	初期化();
	while (1)
	{
		スタートとストップの同時押しでbreak;
		スタートスイッチで motor_on = 1;
		ストップスイッチで motor_on = 0;
		モータ制御(motor_onとトグルスイッチでモータの正転・逆転・停止を制御);
		モータ回転数(モータの回転を検出);
		数値LEDに回転数を表示;
		led(led_pattern);
		ledexec();
		if (debug)
		{
			motor_rotation_countの値を表示する;
		}
	}
	後処理();
	利用方法の表示();
	return 0;
}

/* pad2ps: newpage */

詳細 初期化()
{
	変数の宣言と初期化;
	argvから「 -N」オプション（点灯パターン）を読み取りled_patternに代入;
	argvから「 -F」オプション（点滅モード）を読み取りled_blink_modeに代入;
	argvから「 -B」オプション（点滅周期）を読み取りled_blink_periodに代入;
	argvから「 -DEBUG」オプション（デバッグモード）を読み取りdebugを設定;

  点灯パターン、点滅モード、点滅周期の表示;
  H8ポートの初期化;

  init_timer0(COUNT_100MSEC, null);
  led(led_pattern);
  ledblinkmode(led_blink_mode);
  ledblinktime(led_blink_period);
}

詳細 後処理()
{
	motor_stop();
	stop_timer0();
	ledout(0);
	number_led_off();
}

/* pad2ps: newpage */

詳細 利用方法の表示()
{
	usage： %s(argv[0]) -Bxxx -Fy -Nzz -DEBUG1;
	xxx： 点滅周期[msec];
	y： 0～3の点滅モード;
	zz： 点灯パターン;
	DEBUG1： デバッグモードを有効にする;
}
