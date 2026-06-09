const int SERIAL_BAUD_RATE = 9600;

// カエルの歌（音名） - 休符（"REST"）を挿入
const String MELODY[] = {
  "C4","D4","E4","F4","E4","D4","C4", "REST", // カエルのうたが -> (一拍空ける)
  "E4","F4","G4","A4","G4","F4","E4", "REST", // きこえてくるよ -> (一拍空ける)
  "C4", "REST","C4", "REST","C4", "REST","C4", "REST",                       // クワ クワ クワ クワ
  "C4","C4","D4","D4","E4","E4","F4","F4","E4","D4","C4"         // ケケケケ...
};

// 次の音（または休符）へ進むための基準の長さ（秒）
const float STEP_INTERVALS[] = {
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, // "REST" も1拍分(0.8秒)待つ
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
  0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8
};

// 実際に音が鳴っている長さ（秒）
const float ACTUAL_PLAYS[] = {
  0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.0, // "REST" のときは発音時間を 0.0 秒にする
  0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.0,
  0.3, 0.0, 0.3, 0.0, 0.3, 0.0, 0.3, 0.0,
  0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.7, 0.7, 0.7
};

// 音量（0.0〜1.0）
const float AMPLITUDES[] = {
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.0, // "REST" のときは音量も 0.0 にする
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.0,
  0.9, 0.0, 0.9, 0.0, 0.9, 0.0, 0.9, 0.0,
  0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8
};

// (これ以降の setup, loop 処理は前回提示したコードのままで大丈夫です)
const int NUM_NOTES = sizeof(MELODY) / sizeof(MELODY[0]);
int noteIndex = 0;
bool isPlaying = false; 

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {
  if (!isPlaying) {
    if (Serial.available() > 0) {
      char cmd = Serial.read();
      if (cmd == 'S') { isPlaying = true; noteIndex = 0; }
    }
    return; 
  }

  Serial.print(ACTUAL_PLAYS[noteIndex]);
  Serial.print(",");
  Serial.print(AMPLITUDES[noteIndex]);
  Serial.print(",");
  Serial.println(MELODY[noteIndex]);

  float waitTime = STEP_INTERVALS[noteIndex];
  noteIndex++;
  if (noteIndex >= NUM_NOTES) { isPlaying = false; noteIndex = 0; }
  delay(waitTime * 1000);
}