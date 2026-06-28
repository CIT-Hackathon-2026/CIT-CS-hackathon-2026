// ============================================================
// motor_control.ino (Arduino Uno R4 対応版)
// ステッピングモータ（SM-42BYG011 + L6470）の制御モジュール
// SPI接続: CS=D10, SDI=D11(MOSI), CK=D13(SCK), SDO=D12(MISO)
// ============================================================

#include <SPI.h>

// ---------- 定数 ----------
static const int CS_PIN = 10;  // L6470 チップセレクトピン

static const float TEMPO_MIN  = 40.0f;
static const float TEMPO_MAX  = 200.0f;
static const int   MIN_STEP   = 3;   // [step/s]
static const int   MAX_STEP   = 20;  // [step/s]

// L6470 レジスタ・コマンド定義
static const byte CMD_RUN_FWD  = 0x51;  // RUN(FWD)コマンド上位バイト
static const byte CMD_HARD_STOP = 0xB8; // HARD_STOPコマンド
static const byte CMD_RESET_DEV = 0xC0; // RESET_DEVICEコマンド

// ---------- メンバ変数 ----------
static int  stepSpeed = 0;         // 現在の出力ステップ速度 [step/s]
static int  minStep   = MIN_STEP;  // 最小ステップ速度
static int  maxStep   = MAX_STEP;  // 最大ステップ速度
static bool isRunning = false;     // モータ動作状態フラグ

// 【追加】Uno R4用SPI設定（クロック1MHz, MSBFIRST, SPIモード3）
static SPISettings l6470SPISettings(1000000, MSBFIRST, SPI_MODE3);

// ---------- 内部ヘルパ ----------

// L6470のRUNコマンド送信
static void l6470Run(int speed) {
    uint32_t spd = (uint32_t)(speed * 67);
    if (spd > 0xFFFFF) spd = 0xFFFFF;  // 20bitクランプ

    SPI.beginTransaction(l6470SPISettings); // SPI通信開始
    digitalWrite(CS_PIN, LOW);
    
    SPI.transfer(CMD_RUN_FWD);          // RUN FWD
    SPI.transfer((spd >> 16) & 0x0F);   // 上位4bit
    SPI.transfer((spd >>  8) & 0xFF);   // 中位8bit
    SPI.transfer( spd        & 0xFF);   // 下位8bit
    
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();               // SPI通信終了
}

// L6470にHARD STOPを送信
static void l6470Stop() {
    SPI.beginTransaction(l6470SPISettings);
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(CMD_HARD_STOP);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

// L6470初期化（デバイスリセット）
static void l6470Init() {
    SPI.beginTransaction(l6470SPISettings);
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(CMD_RESET_DEV);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    delay(10);
}

// ---------- 公開関数 ----------

// モータ制御系の初期化（setup()から呼ぶ）
void motorSetup() {
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);

    SPI.begin(); // SPIの開始（設定は各通信時にbeginTransactionで行う）

    l6470Init();
}

// モータを現在のstepSpeedで回転開始する
void startMotor() {
    isRunning = true;
    l6470Run(stepSpeed);
}

// モータを停止する
void stopMotor() {
    isRunning = false;
    stepSpeed = 0;
    l6470Stop();
}

// テンポに応じてステップ速度を計算しモータに反映する
void updateSpeed(float tempo) {
    stepSpeed = mapTempoToStep(tempo);
    applySpeed();
}

// テンポ[BPM]をステップ速度[step/s]に線形変換する
int mapTempoToStep(float tempo) {
    float t = (tempo - TEMPO_MIN) / (TEMPO_MAX - TEMPO_MIN);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    int spd = (int)(minStep + t * (float)(maxStep - minStep));
    return spd;
}

// 現在のstepSpeedをL6470に送信してモータ速度を反映する
void applySpeed() {
    if (!isRunning) return;
    l6470Run(stepSpeed);
}