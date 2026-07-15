// ==========================================
// KICK (打撃クリック＋重低音トーン)
// ==========================================
class KickInstrument implements Instrument {
  Oscil  tone;
  Noise  click;
  ADSR   toneEnv, clickEnv;
  Line   pitchSweep;
  Summer sum;

  KickInstrument() {
    sum = new Summer();
    
    // 1. トーン部分（150Hzから45Hzへ急降下）
    tone = new Oscil(150, 1.0, Waves.SINE);
    toneEnv = new ADSR(0.8, 0.001, 0.1, 0.0, 0.1); 
    pitchSweep = new Line(0.08, 150, 45);
    
    // 2. クリック部分（打撃の瞬間ノイズ）
    click = new Noise(Noise.Tint.WHITE);
    clickEnv = new ADSR(0.4, 0.001, 0.02, 0.0, 0.01);
    
    // 接続
    pitchSweep.patch(tone.frequency);
    tone.patch(toneEnv).patch(sum);
    click.patch(clickEnv).patch(sum);
  }

  void noteOn(float d) { 
    sum.patch(out);
    toneEnv.noteOn(); 
    clickEnv.noteOn(); 
  }
  
  void noteOff() { 
    toneEnv.noteOff(); 
    clickEnv.noteOff(); 
    sum.unpatch(out); 
  }
}

// ==========================================
// SNARE (ノイズ＋スナップ音)
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
// CYMBAL (高周波メタルノイズ風味)
// ==========================================
class CymbalInstrument implements Instrument {
  Noise  noise;
  ADSR   adsr;
  CymbalInstrument() {
    adsr = new ADSR(0.8, 0.01, 0.1, 0.007, 0.008);  
    noise = new Noise(Noise.Tint.WHITE);
    noise.patch(adsr);
  }
  void noteOn(float dur) { adsr.patch(out); adsr.noteOn(); }
  void noteOff() { adsr.unpatchAfterRelease(out); adsr.noteOff(); }
}
