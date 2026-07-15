#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#define CONTROL_RATE 64

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOscil(SIN2048_DATA);

const int baseFreqs[4] = {440, 494, 523, 587};  // 4つの基本周波数

const unsigned long noteOnDuration = 500;   // 音を鳴らす時間（ms）
const unsigned long noteOffDuration = 250;  // 無音時間（ms）

String inputBuffer = "";
int currentIndex = 0;
bool isPlaying = false;

enum PlayPhase {
  PHASE_FIRST_FREQ,
  PHASE_SILENCE_1,
  PHASE_SECOND_FREQ,
  PHASE_SILENCE_2,
  PHASE_NEXT_CHAR
};

PlayPhase playPhase = PHASE_FIRST_FREQ;
unsigned long phaseStartTime = 0;

byte currentHighNibble = 0;
byte currentLowNibble = 0;

// フェーズ内の現在鳴らしている周波数（0なら無音）
int currentFreq = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("文字を入力してください:");
  startMozzi(CONTROL_RATE);
  aOscil.setFreq(0);
}

void updateControl() {
  // シリアル入力受信開始
  if (!isPlaying && Serial.available() > 0) {
    inputBuffer = Serial.readStringUntil('\n');
    inputBuffer += (char)0xFF;
    inputBuffer += (char)0xFF;
    inputBuffer += (char)0xFF; //予備
    currentIndex = 0;
    isPlaying = true;
    playPhase = PHASE_FIRST_FREQ;
    phaseStartTime = millis();

    byte ascii = (byte)inputBuffer.charAt(currentIndex);
    currentHighNibble = (ascii >> 4) & 0x0F;
    currentLowNibble = ascii & 0x0F;

    Serial.println("送信開始");
  }

  if (isPlaying) {
    unsigned long now = millis();
    unsigned long duration = 0;

    switch (playPhase) {
      case PHASE_FIRST_FREQ:
        currentFreq = baseFreqs[currentHighNibble / 4];
        duration = noteOnDuration;
        break;
      case PHASE_SILENCE_1:
        currentFreq = 0;
        duration = noteOffDuration;
        break;
      case PHASE_SECOND_FREQ:
        currentFreq = baseFreqs[currentHighNibble % 4];
        duration = noteOnDuration;
        break;
      case PHASE_SILENCE_2:
        currentFreq = 0;
        duration = noteOffDuration;
        break;
      case PHASE_NEXT_CHAR:
        // 次のニブルに切り替え
        currentFreq = 0;
        // 今度は下位4ビットの処理に切り替え
        currentHighNibble = currentLowNibble;

        playPhase = PHASE_FIRST_FREQ;
        phaseStartTime = now;
        return; // 処理終わり
    }

    aOscil.setFreq(currentFreq);

    if (now - phaseStartTime >= duration) {
      phaseStartTime = now;
      // フェーズを進める
      if (playPhase == PHASE_SILENCE_2) {
        // 上位ニブルの処理終わり。今度は下位ニブルに
        if (currentHighNibble == (byte)(inputBuffer.charAt(currentIndex) >> 4) & 0x0F) {
          // 上位ニブルの後、下位ニブルの準備
          byte ascii = (byte)inputBuffer.charAt(currentIndex);
          currentHighNibble = (ascii & 0x0F);  // 下位ニブルへ
          playPhase = PHASE_FIRST_FREQ;
        } else {
          // 下位ニブルの処理終わった
          currentIndex++;
          if (currentIndex >= inputBuffer.length()) {
            isPlaying = false;
            aOscil.setFreq(0);
            Serial.println("送信終了");
            return;
          }
          // 次の文字の上位ニブルへ
          byte ascii = (byte)inputBuffer.charAt(currentIndex);
          currentHighNibble = (ascii >> 4) & 0x0F;
          currentLowNibble = ascii & 0x0F;
          playPhase = PHASE_FIRST_FREQ;
        }
      } else {
        playPhase = static_cast<PlayPhase>(playPhase + 1);
      }
    }
  }
}

int updateAudio() {
  return aOscil.next() * 8;  // 音量調整
}

void loop() {
  audioHook();
}
