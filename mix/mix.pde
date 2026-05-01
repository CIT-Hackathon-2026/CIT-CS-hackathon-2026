import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim;
AudioOutput out;

void setup() {
  size(600, 300);
  minim = new Minim(this);
  out = minim.getLineOut();
}

void draw() {
  background(30);
  fill(255);
  textAlign(CENTER, CENTER);
  text("【FLUTE】 Q,W,E,R... / A,S,D,F...\n" +
       "【DRUM】 Space:Kick / Shift:Snare / H:Hi-Hat\n" +
       "【TOMS】 1:Hi / 2:Low / 3:Floor", width/2, height/2);
}

void keyPressed() {
  // --- フルート設定 ---
  float[] scale = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25,
                   523.25, 587.33, 659.25, 698.46, 783.99, 880.00, 987.77, 1046.50};
  String keys = "qwertyuiasdfghjk";
  int index = keys.indexOf(key);
  if (index != -1) out.playNote(0.0, 0.8, new FluteInstrument(scale[index]));

// --- ドラム操作 ---
  if (key == ' ') {
    out.playNote(0.0, 0.1, new KickInstrument());
  }
  if (keyCode == SHIFT) {
    out.playNote(0.0, 0.1, new SnareInstrument());
  }
  if (key == 'c' || key == 'C') {
    out.playNote(0.0, 1.5, new CymbalInstrument());
  }
  

}

// ==========================================
// KICK (改良版): 重低音重視
// ==========================================
class KickInstrument implements Instrument {
  Oscil osc; ADSR adsr; Line pitch;
  KickInstrument() {
    // Sustainを0.1残し、Releaseを0.2s(200ms)まで伸ばして「胴鳴り」を作る
    adsr = new ADSR(0.9, 0.0002, 0.1, 0.1, 0.2); 
    osc = new Oscil(140, 1.0, Waves.SINE);
    // ピッチが落ちきる時間を少し長くして、重みを出す
    pitch = new Line(0.15, 140, 30); 
    pitch.patch(osc.frequency);
    osc.patch(adsr);
  }
  void noteOn(float d) { adsr.patch(out); adsr.noteOn(); }
  void noteOff() { adsr.unpatchAfterRelease(out); adsr.noteOff(); }
}

// ==========================================
// シンバル
// ==========================================

class CymbalInstrument implements Instrument {
  Noise  noise;
  ADSR   adsr;

  CymbalInstrument() {
    // 【高周波のキレを出す設定】
    // Attack: 0.001s (最速)
    // Decay: 0.05s (一瞬で減衰)
    // Sustain: 0.0 (維持しない)
    // Release: 0.02s (1秒どころか、ほぼ残響ゼロのスパッとした切れ際)
    adsr = new ADSR(0.8, 0.01, 0.1, 0.007, 0.008); 
    
    // 高域が豊富なWHITEノイズを使用
    noise = new Noise(Noise.Tint.WHITE);
    noise.patch(adsr);
  }

  void noteOn(float dur) {
    adsr.patch(out);
    adsr.noteOn();
  }

  void noteOff() {
    adsr.unpatchAfterRelease(out);
    adsr.noteOff();
  }
}


// ==========================================
// SNARE (そのまま)
// ==========================================
class SnareInstrument implements Instrument {
  Noise noise; Oscil body; ADSR adsr; Summer sum;
  SnareInstrument() {
    adsr = new ADSR(0.6, 0.001, 0.1, 0.0, 0.1);
    sum = new Summer();
    noise = new Noise(Noise.Tint.WHITE);
    body = new Oscil(200, 0.4, Waves.SINE);
    noise.patch(sum); body.patch(sum); sum.patch(adsr);
  }
  void noteOn(float d) { adsr.patch(out); adsr.noteOn(); }
  void noteOff() { adsr.unpatchAfterRelease(out); adsr.noteOff(); }
}

// ==========================================
// FLUTE (そのまま)
// ==========================================
class FluteInstrument implements Instrument {
  Oscil wave0, wave1, wave2; ADSR adsr; Summer sum;
  FluteInstrument(float freq) {
    adsr = new ADSR(0.5, 0.1, 0.2, 0.7, 0.2);
    sum = new Summer();
    wave0 = new Oscil(freq, 1.0, Waves.SINE);
    wave1 = new Oscil(freq * 2, 0.4, Waves.SINE);
    wave2 = new Oscil(freq * 3, 0.1, Waves.SINE);
    wave0.patch(sum); wave1.patch(sum); wave2.patch(sum);
    sum.patch(adsr);
  }
  void noteOn(float d) { adsr.patch(out); adsr.noteOn(); }
  void noteOff() { adsr.unpatchAfterRelease(out); adsr.noteOff(); }
}
