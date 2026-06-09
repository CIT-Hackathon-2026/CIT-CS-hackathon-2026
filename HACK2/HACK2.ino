const int SERIAL_BAUD_RATE = 9600;

// 1. カエルの歌（音名）- 35ステップ
const String MELODY[] = {
  "C4","D4","E4","F4","E4","D4","C4", "REST",
  "E4","F4","G4","A4","G4","F4","E4", "REST",
  "C4", "REST","C4", "REST","C4", "REST","C4", "REST",
  "C4","C4","D4","D4","E4","E4","F4","F4","E4","D4","C4"
};

// 2. 次のステップへ進むまでの長さ（秒）- 35ステップ
const float STEP_INTERVALS[] = {
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
  0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8
};

// 3. 実際にフルートの音が鳴っている長さ（秒）- 35ステップ
const float ACTUAL_PLAYS[] = {
  0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.0,
  0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.0,
  0.3, 0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0,
  0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.7, 0.7, 0.7
};

// 4. ドラムパターン - 35ステップ
const String DRUM_PATTERN[] = {
  "KICK", "NONE", "SNARE", "NONE", "KICK", "KICK", "SNARE", "CYMBAL",
  "KICK", "NONE", "SNARE", "NONE", "KICK", "KICK", "SNARE", "CYMBAL",
  "KICK", "CYMBAL", "SNARE", "CYMBAL", "KICK", "CYMBAL", "SNARE", "CYMBAL",
  "KICK", "CYMBAL", "SNARE", "CYMBAL", "KICK", "CYMBAL", "SNARE", "CYMBAL", "KICK", "SNARE", "CYMBAL"
};

// 全体のステップ数（35）を自動計算
const int NUM_STEPS = sizeof(MELODY) / sizeof(MELODY[0]);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  // ⚠️ Processingが起動して接続するのを少し待つための安全時間を追加（1.5秒）
  delay(1500);

  // カエルの歌を1番から35番まで「1回だけ」順番に演奏する
  for (int stepIndex = 0; stepIndex < NUM_STEPS; stepIndex++) {
    
    // Processingへデータを送信
    Serial.print(STEP_INTERVALS[stepIndex]);
    Serial.print(",");
    Serial.print(ACTUAL_PLAYS[stepIndex]);
    Serial.print(",");
    Serial.print(MELODY[stepIndex]);
    Serial.print(",");
    Serial.println(DRUM_PATTERN[stepIndex]);

    // 次のステップまでの待ち時間（ミリ秒）
    int delayTime = STEP_INTERVALS[stepIndex] * 1000;
    delay(delayTime);
  }

  // 演奏が終わったらここに到達し、これ以降は何も送信されなくなります
}

void loop() {
  // setup() で一度だけ演奏させたいため、通常のループ内は完全に空っぽ（何もしない）にします
}