#include "Arduino_LED_Matrix.h"

// ===== フォトダイオード（トリガー側：ubukata）=====
int V_up = 12;   // LED点灯の閾値
int V_down = 5; // LED消灯の閾値
const int s_PIN = A0; // フォトダイオードをつなぐピン
int sensor = 0;       // フォトダイオードで読み取った値
bool isHighState;        // LEDの状態
bool before_isHighState; // 前回のLEDの状態

unsigned long before_time = 0;
unsigned long interval = 0;

// ===== Processingへ送る音データ（送信内容：zissou_pet）=====
const String MELODY[] = {
  "C5", "D5", "E5", "F5", "E5", "D5", "C5", "E5", "F5", "G5", "A5", "G5", "F5",
  "E5", "C5", "C5", "C5", "C5", "C5", "C5", "D5", "D5", "E5", "E5", "F5", "F5",
  "E5", "D5", "C5"
};
const float DURATIONS[] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
  1.0f, 1.0f, 1.0f
};
const float AMPLITUDES[] = {
  0.8, 0.4, 0.8, 0.5, 0.8, 0.9, 1.0, 0.8, 0.4, 0.8, 0.5, 0.8, 0.9
};

// 3配列のうち最も短い長さに合わせて1周させる（配列外アクセス防止）
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
}; // LEDマトリクスに「ok」を表示

// 各配列の要素数を求めるヘルパー
int minArrayLength() {
  int lenMelody    = sizeof(MELODY)    / sizeof(MELODY[0]);
  int lenDurations = sizeof(DURATIONS) / sizeof(DURATIONS[0]);
  int lenAmps      = sizeof(AMPLITUDES)/ sizeof(AMPLITUDES[0]);

  int m = lenMelody;
  if (lenDurations < m) m = lenDurations;
  if (lenAmps      < m) m = lenAmps;
  return m;
}

// Processingへ1音ぶんのCSVを送信する（zissou_petの送信フォーマット）
void sendNote() {
  Serial.print(DURATIONS[noteIndex]);
  Serial.print(",");
  Serial.print(AMPLITUDES[noteIndex]);
  Serial.print(",");
  Serial.println(MELODY[noteIndex]);

  noteIndex++;
  if (noteIndex >= numNotes) {
    noteIndex = 0;
  }
}

void setup() {
  Serial.begin(9600);
  matrix.begin();

  numNotes = minArrayLength();

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
  Serial.println(sensor); // デバッグ用。送信フォーマットと混ざるので通常はコメントアウト

  if (sensor >= V_up) {
    isHighState = true;
  }
  if (sensor <= V_down) {
    isHighState = false;
  }

  // フォトダイオードの状態が切り替わった瞬間をトリガーにする（ubukata）
  if (before_isHighState != isHighState) {
    unsigned long now = millis();
    interval = now - before_time;

    if (interval > 20) {
      sendNote();                    // 中身はzissou_petのCSV送信
    } else {
      matrix.renderBitmap(ok, 8, 12); // 速すぎる切り替わりは「ok」表示
    }

    before_isHighState = isHighState;
    before_time = now;
  }

  delay(5);
}