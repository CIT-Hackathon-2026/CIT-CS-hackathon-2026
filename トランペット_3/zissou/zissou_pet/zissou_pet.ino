#include "Arduino_LED_Matrix.h"

// ===== フォトダイオード=====
// ===== フォトダイオード =====
// この回路は「光ると値が0付近まで下がる」極性．
//   503付近 = 暗い(基準) = true
//   0付近   = 光った       = false  と判定する
int V_up   = 400; // これより大きい → 基準(true)
int V_down = 150; // これより小さい → 光った(false)
const int s_PIN = A0; // フォトダイオードをつなぐピン
int sensor = 0;       // フォトダイオードで読み取った値
bool isHighState;        // LEDの状態
bool before_isHighState; // 前回のLEDの状態

unsigned long before_time = 0;
unsigned long interval = 0;

// ===== タイミング設定 =====
const int TICKS_PER_BEAT = 24; // 1拍 = 24回の光トリガー
int tickCounter = 0;           // 現在の音が鳴り始めてからの経過トリガー数
int ticksThisNote = 0;         // 現在の音が占めるトリガー数
bool started = false;          // 最初の音をすでに送ったか

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
  "E5", "D5", "C5"
};
const float DURATIONS[] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
  1.0f, 1.0f, 1.0f
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
  0.74, 0.70, 0.66
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
  Serial.begin(9600);
  matrix.begin();

  int lenMelody    = sizeof(MELODY)    / sizeof(MELODY[0]);
  int lenDurations = sizeof(DURATIONS) / sizeof(DURATIONS[0]);
  numNotes = lenMelody;
  if (lenDurations < numNotes) numNotes = lenDurations;

  sensor = analogRead(s_PIN);
  if (sensor >= V_up) {
    isHighState = true;
  } else {
    isHighState = false;
  }
  before_isHighState = isHighState;
}

void loop() {
  sensor = analogRead(s_PIN);
  Serial.println(sensor); // ← Processing通信中は必ずコメントアウト（CSVが汚れる）

  if (sensor >= V_up)   isHighState = true;   // 暗い(基準)
  if (sensor <= V_down) isHighState = false;  // 光った

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
        if (tickCounter >= ticksThisNote) {
          noteIndex++;
          if (noteIndex >= numNotes) noteIndex = 0;
          advance = true;
        }
      }

      if (advance) {
        float amp = (MELODY[noteIndex] == "R") ? 0.0 : AMPLITUDES[noteIndex];

        Serial.print(DURATIONS[noteIndex]);
        Serial.print(",");
        Serial.print(amp);
        Serial.print(",");
        Serial.println(MELODY[noteIndex]);

        ticksThisNote = (int)(DURATIONS[noteIndex] * TICKS_PER_BEAT + 0.5f);
        tickCounter = 0;
      }

      before_time = now;
    }
  }

  before_isHighState = isHighState; // ★毎ループ更新（立ち下がり検出ではここが必須）
  delay(1);
}