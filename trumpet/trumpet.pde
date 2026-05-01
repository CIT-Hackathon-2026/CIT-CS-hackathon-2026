import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim;
AudioOutput out;
Waveform currentWaveform; // 音色格納用変数

// 各音の高さ（A3からA5まで半音階で追加）
String[] melody = {
  "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5",
};

// 各音の長さ（拍）
float[] duration = {
  2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f
};

// 各音の開始位置
float[] startTime = {
  0.0f, 2.5f, 5.0f, 7.5f, 10.0f, 12.5f, 15.0f, 17.5f, 20.0f, 22.5f, 25.0f, 27.5f, 30.0f
};

// 音色とエンベロープ（ADSR）を設定するクラス
class HackInstrument implements Instrument {
  Oscil wave;
  ADSR adsr;
  Oscil vibrato;

  HackInstrument(float frequency, float maxAmp, Waveform wf) {
    wave = new Oscil(frequency, 1.0f, wf);
    
    // ① トランペット風のエンベロープ
    adsr = new ADSR(maxAmp, 0.08f, 0.15f, 0.8f, 0.15f); 
    
    // ② ビブラートの追加（5Hzの速さで、上下に3Hz揺らす）
    vibrato = new Oscil(5.0f, 3.0f, Waves.SINE);
    
    // ビブラートの「中心の高さ」を元の周波数に設定する
    vibrato.offset.setLastValue(frequency);
    
    // ビブラートをメインの波形の周波数(frequency)に接続
    vibrato.patch(wave.frequency);
    
    // エンベロープを接続
    wave.patch(adsr);
  }

  void noteOn(float duration) {
    adsr.patch(out);
    adsr.noteOn();
  }

  void noteOff() {
    adsr.noteOff();
    adsr.unpatchAfterRelease(out);
  }
}

void setup() {
  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut();
  out.setTempo(120);

  // 起動時からトランペット風の倍音構成に固定
  currentWaveform = WavetableGenerator.gen10(
    4096, 
    new float[] { 0.7f, 1.0f, 0.9f, 0.7f, 0.5f, 0.3f, 0.2f, 0.15f, 0.1f, 0.05f } 
  );
}

void playSong() {
  out.pauseNotes(); 

  for (int i = 0; i < melody.length; i++) {
    out.playNote(startTime[i], duration[i],
      new HackInstrument(Frequency.ofPitch(melody[i]).asHz(), 0.5f, currentWaveform));
  }
  
  out.resumeNotes();
}

void draw() {
  background(0);
  stroke(255);

  for(int i = 0; i < out.bufferSize() - 1; i++) {
    line(i, 50 + out.left.get(i)*50, i+1, 50 + out.left.get(i+1)*50);
    line(i, 150 + out.right.get(i)*50, i+1, 150 + out.right.get(i+1)*50);
  }
}

void keyPressed() {
  // 1〜6キーの音色変更処理を削除し、pキーでの再生のみを残す
  if (key == 'p' || key == 'P') {
    playSong();
  }
}
