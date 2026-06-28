//コメントアウト部分は適宜解除してください
//このコードはモーターへの還元機能がありませんので留意してください

// ---------- 状態定数 ----------
static const int INIT = 0;
static const int WAIT = 1;
static const int RUN  = 2;
static const int END  = 3;

// ---------- 設定値（曲ごとに変更） ----------
static const long TOTAL_PULSE_THRESHOLD = 6000;

// ★輪唱のタイミング設定
static const int DELAY_BEATS = 4;       // 後の子機を何拍遅らせるか
static const int PULSES_PER_BEAT = 24;  // 1拍あたりのパルス数

// ★固定テンポ（ここの数字を変えれば曲の速さが変わる）
static const float FIXED_TEMPO = 60.0;

// ---------- 定数 ----------
static const int BUTTON_PIN = 2;  // 開始ボタンピン
static const int LED_PINS[4] = {3, 4, 5, 6};

// ---------- メンバ変数 ----------
static int           state     = INIT;
static unsigned long startTime = 0;
static bool          isStarted = false;
static unsigned long previousPulseTime = 0;
static long          currentPulseCount = 0;

// LEDを止めずに消すための変数
static bool          ledIsOn       = false;
static unsigned long ledStartTime  = 0;
static int           ledOnDuration = 0;

void setup() {
    Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    for (int i = 0; i < 4; i++) {
        pinMode(LED_PINS[i], OUTPUT);
    }

    motorSetup();

    changeState(WAIT);
    Serial.println("State: WAIT");
}

void loop() {
    handleState();
}

void changeState(int newState) {
    state = newState;
}

void handleState() {
    switch (state) {

        // ---- 待機状態：ボタン入力を監視 ----
        case WAIT:
            if (digitalRead(BUTTON_PIN) == LOW) {
                delay(50);  // チャタリング除去
                if (digitalRead(BUTTON_PIN) == LOW) {
                    isStarted = true;
                    startTime = millis();
                    currentPulseCount = 0;
                    updateSpeed(FIXED_TEMPO); // ★固定テンポでモーター速度を設定
                    startMotor();
                    changeState(RUN);
                    Serial.println("State: RUN");
                }
            }
            break;

        // ---- 動作状態 ----
        case RUN: {
            unsigned long currentMillis = millis();

            // 固定テンポから間隔を計算（ミリ秒）
            float interval_ms = 60000.0 / FIXED_TEMPO / 24.0;

            //------一時的コメントアウト⭐️------一//
            //bool detected = false;
            //------一時的コメントアウト⭐️------一//

            if (currentMillis - previousPulseTime >= interval_ms) {
                //previousPulseTime = currentMillis;
                previousPulseTime += interval_ms;

                // ====== 🔍 ここからシリアルモニター用の出力表示 ======
                //Serial.print("RUN - Pulse: ");
                //Serial.print(currentPulseCount);
                //Serial.print(" / ");
                //Serial.print(TOTAL_PULSE_THRESHOLD);
                //Serial.println(" | Active: ");
                // ===================================================

                // LEDを点ける
                for (int i = 0; i < 4; i++) {
                    long startThreshold = i * DELAY_BEATS * PULSES_PER_BEAT;
                    if (currentPulseCount >= startThreshold) {
                        digitalWrite(LED_PINS[i], HIGH);
                    // 一時追加地点    
                    } else {
                        digitalWrite(LED_PINS[i], LOW);  // 自分の番が来るまでは「絶対にLOW」で固定！
                    }
                    // 一時追加地点    
                }

                // delayの代わり：点けた時刻と点灯時間を記録（ループは止めない）
                ledIsOn       = true;
                ledStartTime  = currentMillis;
                ledOnDuration = (int)(interval_ms / 2);

                currentPulseCount++;
                sendPulse();
            //------一時的コメントアウト⭐️------一//
                //detected = true;
            //------一時的コメントアウト⭐️------一//
            }

            //------一時的コメントアウト⭐️------一//
            // LEDを消す処理：時間が来たら消す
            //if (ledIsOn && (currentMillis - ledStartTime >= ledOnDuration)) {
                //for (int i = 0; i < 4; i++) {
                    //digitalWrite(LED_PINS[i], LOW);
                //}
                //ledIsOn = false;
            //}
            //------一時的コメントアウト⭐️------一// 

            //------一時追加・修正必須箇所⭐️------一//
            // LEDを消す処理：時間が来たら消す
            if (ledIsOn && (currentMillis - ledStartTime >= ledOnDuration)) {
                // ⭕️【修正】すでにスタートしているLED「だけ」を消灯させる
                for (int i = 0; i < 4; i++) {
                    long startThreshold = i * DELAY_BEATS * PULSES_PER_BEAT;
                    if (currentPulseCount >= startThreshold) {
                        digitalWrite(LED_PINS[i], LOW);
                    }
                }
                ledIsOn = false;
            }
            //------一時追加・修正必須箇所⭐️------一//

            //------一時的コメントアウト⭐️------一//
            //if (detected) {
                //updateTiming(); // 総パルス数のカウントだけ進める

                //if (getTotalPulseCount() >= TOTAL_PULSE_THRESHOLD) {
                    //stopMotor();
                    //changeState(END);
                    //Serial.println("State: END");
                //}
            //}
            //break;
            //------一時的コメントアウト⭐️------一//

            //------一時追加・修正必須箇所⭐️------一//
            if (currentPulseCount >= TOTAL_PULSE_THRESHOLD) {
                // stopMotor(); // モーターは回さないのでコメントアウト（⭐️）
                changeState(END);
                Serial.println("State: END");
            }
            break;
            //------一時追加・修正必須箇所⭐️------一//

        }

        case END:
            isStarted = false;
            startTime = 0;
            changeState(WAIT);
            Serial.println("State: WAIT");
            break;

        default:
            break;
    }
}