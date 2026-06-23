#include "Arduino_LED_Matrix.h"

// ===== フォトダイオード=====
int V_up = 12;   // LED点灯の閾値
int V_down = 5; // LED消灯の閾値
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
  // Serial.println(sensor); // デバッグ用。CSV送信と混ざってProcessing側が誤読するので通常はコメントアウト

  if (sensor >= V_up) {
    isHighState = true;
  }
  if (sensor <= V_down) {
    isHighState = false;
  }

  // フォトダイオードの状態が切り替わった瞬間をトリガーにする
  if (before_isHighState != isHighState) {
    unsigned long now = millis();
    interval = now - before_time;

    if (interval > 20) {
      bool advance = false;

      if (!started) {
        // 最初のトリガーで1音目を送る
        noteIndex = 0;
        started = true;
        advance = true;
      } else {
        // 1トリガー = 1/24拍。経過を数える
        tickCounter++;
        if (tickCounter >= ticksThisNote) {
          // 現在の音が「音の長さ×24回」分鳴り終わった → 次の音へ
          noteIndex++;
          if (noteIndex >= numNotes) {
            noteIndex = 0;
          }
          advance = true;
        }
      }

      if (advance) {
        // 休符は無音(0.0)
        float amp;
        if (MELODY[noteIndex] == "R") {
          amp = 0.0;
        } else {
          amp = AMPLITUDES[noteIndex];
        }

        // 1音分送信
        Serial.print(DURATIONS[noteIndex]);
        Serial.print(",");
        Serial.print(amp);
        Serial.print(",");
        Serial.println(MELODY[noteIndex]);

        //音の長さ×24（四捨五入）
        ticksThisNote = (int)(DURATIONS[noteIndex] * TICKS_PER_BEAT + 0.5f);
        tickCounter = 0;
      }
    } else {
      matrix.renderBitmap(ok, 8, 12); 
    }

    before_isHighState = isHighState;
    before_time = now;
  }

  delay(5);
}