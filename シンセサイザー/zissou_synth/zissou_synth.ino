// 🎵 命名規約に従い、全ての大文字定数を設定
const int   SERIAL_BAUD_RATE = 9600;
const int   NOTE_INTERVAL_MS = 2500; // 次の音までの待機時間
const int   NUM_NOTES = 13;

const String MELODY[] = {"A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5"};
const float  DURATIONS[] = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

// ご指定の配列（C++の形式に合わせ、定数名も大文字に統一しています）
const float START_TIME[] = {
  0.0, 2.5, 5.0, 7.5, 10.0, 12.5, 15.0, 17.5, 20.0, 22.5, 25.0, 27.5, 30.0
};

// 変数はcamelCase
int noteIndex = 0; 

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {
  // ★光センサーを使う場合は、この上に analogRead と if文の判定を入れます
  
  if (noteIndex < NUM_NOTES) {
    // データをカンマ区切りで送信 ("長さ, 音の強さ, 音名" の3つになります)
    Serial.print(DURATIONS[noteIndex]);
    Serial.print(",");
    Serial.print(START_TIME[noteIndex]); // ★追加した配列の値を送信
    Serial.print(",");
    Serial.println(MELODY[noteIndex]);
    
    noteIndex++; 
    
    // 配列の最後まで到達したら最初に戻る（ループ再生）
    if (noteIndex >= NUM_NOTES) {
      noteIndex = 0;
    }
    
    delay(NOTE_INTERVAL_MS); 
  }
}