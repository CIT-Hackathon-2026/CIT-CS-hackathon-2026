// ---------- 状態定数 ----------
static const int INIT = 0;
static const int WAIT = 1;
static const int RUN  = 2;
static const int END  = 3;

// ---------- 設定値（曲ごとに変更） ----------
// 総パルス数の閾値 = 拍数 × 24
// 120BPM × 30秒 = 60拍 → 60 × 24 = 1440パルス
static const long TOTAL_PULSE_THRESHOLD = 1440;

// ---------- 定数 ----------
static const int BUTTON_PIN = 2;  // 開始ボタンピン

// ---------- メンバ変数 ----------
static int           state     = INIT;   // 現在の状態
static unsigned long startTime = 0;      // 動作開始時刻 [ms]
static bool          isStarted = false;  // 開始済みフラグ


void setup() {
    Serial.begin(9600);

    // ボタンピン設定（内部プルアップ使用）
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // モータ制御系初期化
    motorSetup();

    // 状態をWAITへ
    changeState(WAIT);

    Serial.println("State: WAIT");
}

void loop() {
    handleState();
}

// 状態をnewStateに変更する
void changeState(int newState) {
    state = newState;
}

// 現在の状態に応じた処理を実行する
void handleState() {
    switch (state) {

        // ---- 待機状態：ボタン入力を監視 ----
        case WAIT:
            // INPUT_PULLUPのためボタン押下でLOW
            if (digitalRead(BUTTON_PIN) == LOW) {
                delay(50);  // チャタリング除去
                if (digitalRead(BUTTON_PIN) == LOW) {
                    isStarted = true;
                    startTime = millis();
                    startMotor();
                    changeState(RUN);
                    Serial.println("State: RUN");
                }
            }
            break;

        // ---- 動作状態：パルス受信・テンポ算出・モータ制御 ----
        case RUN: {
            //LEDの送信プログラムをここに入れる
            // パルスを検知したか
            bool detected = sendPulse();

            if (detected) {
                // パルス間隔の移動平均を更新
                measureInterval();

                // カウントアップ・更新タイミング判定
                bool shouldUpdate = updateTiming();

                if (shouldUpdate) {
                    // 12パルスごとにテンポ再計算・速度更新
                    float currentTempo = calculateTempo();
                    updateSpeed(currentTempo);

                    Serial.print("Tempo: ");
                    Serial.print(currentTempo);
                    Serial.print(" BPM  StepSpeed: ");
                    Serial.println(mapTempoToStep(currentTempo));
                }

                // 終了判定
                if (getTotalPulseCount() >= TOTAL_PULSE_THRESHOLD) {
                    stopMotor();
                    changeState(END);
                    Serial.println("State: END");
                }
            }
            break;
        }

        // ---- 終了状態：一部変数をリセットして待機状態へ戻る ----
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
