#include "Arduino_LED_Matrix.h"

// ===== フォトダイオード =====
// この回路は「光ると値が0付近まで下がる」極性．
//   503付近 = 暗い(基準) = true
//   0付近   = 光った       = false  と判定する
int V_up   = 200; // これより大きい → 基準(true)
int V_down = 100; // これより小さい → 光った(false)
const int s_PIN = A0; // フォトダイオードをつなぐピン
int sensor = 0;       // フォトダイオードで読み取った値
bool isHighState;        // LEDの状態
bool before_isHighState; // 前回のLEDの状態

unsigned long before_time = 0;
unsigned long interval = 0;

// ===== タイミング設定 =====
const int TICKS_PER_BEAT = 24; // 1拍 = 24回の光トリガー
int tickCounter = 0;           // 現在の音が鳴り始めてからの経過トリガー数
int ticksThisNote = 0;         // 現在の音が占めるトリガー数（次の音へ進む間隔）
bool started = false;          // 最初の音をすでに送ったか

// ★追加：発音時間の管理（PLAY_DURATIONS × TICKS_PER_BEAT ぶん光を数える）
int playTicks = 0;             // この音を鳴らし続ける光の回数
bool notePlaying = false;      // 今、音を鳴らしている最中か

// ===== Processingへ送る音データ=====
// 休符は "R"（無音）として配列に組み込んでいる．
const String MELODY[] = {
  "C5", "D5", "E5", "F5", "E5", "D5", "C5", "R",
  "E5", "F5", "G5", "A5", "G5", "F5",
  "E5", "R",
  "C5", "R",
  "C5", "R",
  "C5", "R",
  "C5", "R",
  "C5", "C5", "D5", "D5", "E5", "E5", "F5", "F5",
  "E5", "D5", "C5",
  "R",
  "C5", "R",
  "C5", "R",
  "C5", "R",
  "C5", "R",
  "C5", "C5", "D5", "D5", "E5", "E5", "F5", "F5",
  "E5", "D5", "C5",
  "R"
};
// DURATIONS = 「次の音を出すまでの時間（=この音が占める拍数）」
//   ticksThisNote の計算に使う（音の間隔・テンポを決める）
const float DURATIONS[] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
  1.0f, 1.0f, 1.0f,
  1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
  1.0f, 1.0f, 1.0f,
  1.0f
};
// PLAY_DURATIONS = 「実際に音を流す時間（拍数）」
//   playTicks の計算に使う（この拍数ぶんの光を数えて STOP を送る）
const float PLAY_DURATIONS[] = {
  0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 1.0f, 0.9f,
  0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f,
  1.0f, 0.7f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
  0.9f, 0.9f, 1.0f,
  1.0f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.9f, 0.7f,
  0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
  0.9f, 0.9f, 1.0f,
  1.0f
};
const float AMPLITUDES[] = {
  0.78, 0.78, 0.80, 0.82, 0.80, 0.78, 0.76, 0.00,
  0.78, 0.80, 0.82, 0.84, 0.82, 0.80,
  0.78, 0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.66, 0.66, 0.68, 0.68, 0.70, 0.70, 0.72, 0.72,
  0.74, 0.70, 0.66,
  0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.86, 0.00,
  0.66, 0.66, 0.68, 0.68, 0.70, 0.70, 0.72, 0.72,
  0.74, 0.70, 0.66,
  0.00
};

int noteIndex = 0;
int numNotes;  // setup内で算出

// ===== LEDマトリクス =====
ArduinoLEDMatrix matrix;
byte ok [8][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0, 0, 1, 1, 1, 0, 0, 1, 0,0,1,0},
  {0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
  {0,0,0,0,0,0,0,0,0,0,0,0}
};

void setup() {
  Serial.begin(115200);
  matrix.begin();

  int lenMelody    = sizeof(MELODY)    / sizeof(MELODY[0]);
  int lenDurations = sizeof(DURATIONS) / sizeof(DURATIONS[0]);
  int lenPlayDur   = sizeof(PLAY_DURATIONS) / sizeof(PLAY_DURATIONS[0]);
  numNotes = lenMelody;
  if (lenDurations < numNotes) numNotes = lenDurations;
  if (lenPlayDur   < numNotes) numNotes = lenPlayDur;

  sensor = analogRead(s_PIN);
  if (sensor >= V_down) {   // ★向きを反転：0付近=受信した(true)
    isHighState = true;
  } else {
    isHighState = false;
  }
  before_isHighState = isHighState;
}


void loop() {
  sensor = analogRead(s_PIN);
  //Serial.println(sensor); // ← Processing通信中は必ずコメントアウト（CSVが汚れる）

  if (sensor >= V_down) {   // ★向きを反転：0付近=受信した(true)
    isHighState = true;
  }
  if (sensor <= V_up) {     // ★向きを反転：1023付近=受信なし(false)
    isHighState = false;
  }

  // 「暗い(true) → 光った(false)」の瞬間だけをトリガーにする（1フラッシュ=1トリガー）
  if (before_isHighState == true && isHighState == false) {
    unsigned long now = millis();
    interval = now - before_time;

    if (interval > 20) { // チャタリング除去
      bool advance = false;

      if (!started) {
        noteIndex = 0;
        started = true;
        advance = true;
      } else {
        tickCounter++;

        // ★追加：発音時間ぶん光を数えたら STOP を送る（一度だけ）
        if (notePlaying && tickCounter >= playTicks) {
          Serial.println("STOP");
          notePlaying = false;
        }

        // 次の音符へ進むか判定（今まで通り DURATIONS 基準）
        if (tickCounter >= ticksThisNote) {
          noteIndex++;
          if (noteIndex >= numNotes) noteIndex = 0;
          advance = true;
        }
      }

      if (advance) {
        float amp = (MELODY[noteIndex] == "R") ? 0.0 : AMPLITUDES[noteIndex];

        // Processing へ音符データを送る（長さ, 振幅, 音名）
        // ※実際の発音停止は STOP で行うが、互換のため長さも送っている
        Serial.print(PLAY_DURATIONS[noteIndex]);
        Serial.print(",");
        Serial.print(amp);
        Serial.print(",");
        Serial.println(MELODY[noteIndex]);

        // ★この音を鳴らし続ける光の回数を計算して覚える
        playTicks = (int)(PLAY_DURATIONS[noteIndex] * TICKS_PER_BEAT + 0.5f);
        notePlaying = true;

        // 次の音へ進むまでの間隔は「DURATIONS」で決める
        ticksThisNote = (int)(DURATIONS[noteIndex] * TICKS_PER_BEAT + 0.5f);
        tickCounter = 0;
      }

      before_time = now;
    }
  }

  before_isHighState = isHighState; // ★毎ループ更新（立ち下がり検出ではここが必須）
  delay(1);
}