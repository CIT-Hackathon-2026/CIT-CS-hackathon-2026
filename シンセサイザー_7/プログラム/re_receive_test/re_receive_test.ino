#include <arduinoFFT.h>

#define SAMPLES 512
#define SAMPLING_FREQUENCY 5000
#define INPUT_PIN A0
#define mbits 256

#define pin 13
#define duration 5000 //1ビットごとの送る速度(µs)
#define start 160000 //システムを開始するための点灯時間(µs)
#define end_count 10 //システムを終了するための符号の数

const double NOISE_THRESHOLD = 2000.0;
const double MIN_VALID_FREQ = 300.0;
const int CONFIRM_COUNT = 2;  // 周波数検出の安定確認

ArduinoFFT<float> FFT = ArduinoFFT<float>();

float vReal[SAMPLES];
float vImag[SAMPLES];

const int baseFreqs[4] = {440, 494, 523, 587};  // 基本周波数（4つ）

bool isUpperNibble = true;
byte upperNibble = 0;
String received = "";
int bits[mbits];
int bitcount = 0;

int bit = 0;
int length = 0;

int end[1] = {1}; //終了信号フラグ
unsigned long pretime = 0;
int state = 0;
int endbit = 0;

char key = 'a';

// 検出補助
int lastDetectedNibble = -1;
int consecutiveCount = 0;

// 周波数ペア保持用
int detectedPair[2];
int pairIndex = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  pretime = micros();
  Serial.println("FFT受信開始");
}

void loop() {
  unsigned long startMicros = micros();

  // FFT入力収集
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(INPUT_PIN);
    vImag[i] = 0;
    delayMicroseconds(1000000 / SAMPLING_FREQUENCY);
  }

  unsigned long endMicros = micros();
  float actualSamplingFreq = (float)SAMPLES / ((endMicros - startMicros) / 1000000.0);

  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  double maxMagnitude = 0;
  int maxIndex = 0;
  for (int i = 5; i < SAMPLES / 2; i++) {
    if (vReal[i] > maxMagnitude) {
      maxMagnitude = vReal[i];
      maxIndex = i;
    }
  }

  if (maxMagnitude < NOISE_THRESHOLD) {
    Serial.println("→ 無音またはノイズ → スキップ");
    resetDetection();
    delay(100);
    return;
  }

  double peakFrequency = (maxIndex * actualSamplingFreq) / SAMPLES;

  if (peakFrequency < MIN_VALID_FREQ) {
    Serial.print("低周波スキップ: ");
    Serial.println(peakFrequency);
    resetDetection();
    delay(100);
    return;
  }

  // 周波数マッチング
  int matchedFreqIndex = -1;
  double minDiff = 99999;
  for (int i = 0; i < 4; i++) {
    double diff = abs(baseFreqs[i] - peakFrequency);
    if (diff < minDiff) {
      minDiff = diff;
      matchedFreqIndex = i;
    }
  }

  if (matchedFreqIndex != -1) {
    Serial.print("検出周波数: ");
    Serial.print(peakFrequency, 2);
    Serial.print(" Hz → Index: ");
    Serial.println(matchedFreqIndex);

    if (matchedFreqIndex == lastDetectedNibble) {
      consecutiveCount++;
    } else {
      lastDetectedNibble = matchedFreqIndex;
      consecutiveCount = 1;
    }

    if (consecutiveCount >= CONFIRM_COUNT) {
      Serial.println("→ 確定検出");

      detectedPair[pairIndex++] = matchedFreqIndex;

      if (pairIndex == 2) {
        int a = detectedPair[0];
        int b = detectedPair[1];
        byte nibble = (a << 2) | b;  // 4bit作成

        Serial.print("Nibble: ");
        Serial.print(nibble, HEX);
        Serial.print(" (");
        Serial.print(nibble);
        Serial.println(")");

        if (isUpperNibble) {
          upperNibble = nibble;
          isUpperNibble = false;
        } else {
          byte ascii = (upperNibble << 4) | nibble;
          received += (char)ascii;

          Serial.print("復元文字: ");
          Serial.print((char)ascii);
          Serial.print(" (");
          Serial.print(ascii);
          Serial.println(")");

          Serial.print("2進数: ");
          for (int i = 7; i >= 0; i--) {
            int bit = (ascii >> i) & 1;
            Serial.print(bit);
            bits[bitcount++] = bit;
            length++;
          }
          Serial.println();
          Serial.print("bits配列: ");
          for (int i = 0; i < bitcount; i++) {
            Serial.print(bits[i]);
            Serial.print(" ");
          }
          Serial.println("\n----------------------");

          if (ascii == 0xFF) {
            Serial.println("==終了信号==");
            key = 'b';
          }

          isUpperNibble = true;
        }

        pairIndex = 0;
        resetDetection();
      }
    }
  } else {
    resetDetection();
  }

  delay(100);

  if (key == 'b') {
    while (state < 4) {
      signal();  // 送信フェーズへ
    }
    while (true);  // 完全停止
  }
}

void resetDetection() {
  lastDetectedNibble = -1;
  consecutiveCount = 0;
}


void signal(){
  unsigned long now = micros(); //システムの時間
  
  if(state == 0 && length > 0){ //信号が送られるまで待機させる
    Serial.println("送信開始");
    pretime = micros();
    state = 1;
  }

  switch (state){
    case 1: //開始信号
      digitalWrite(pin, HIGH);
      if(now - pretime >= start){ //システムの時間がstartの時間を超えたら
        pretime = now;
        state = 2;
        bit = 0;
        endbit = 0;
      }
      delay(190);
      break;
    
    case 2: //信号の伝達
      if(now - pretime >= duration){
        if(bit < length){
          if(bits[bit] == 1){
            digitalWrite(pin, HIGH);
          } else {
            digitalWrite(pin, LOW);
          }
          bit++;
          pretime = now;
        } else {
          state = 3;
          pretime = now;
        }
      }
      break;

    case 3: //終了信号
      if (now - pretime >= duration) {
        if (endbit < end_count) {
          digitalWrite(pin, end[endbit] ? HIGH : LOW);
          endbit++;
          pretime = now;
        } else {
          digitalWrite(pin, LOW);
          Serial.println("送信完了");
          state = 4;
        }
      }
      break;
    
    case 4:

      break;
  }
}
