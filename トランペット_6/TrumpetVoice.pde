class TrumpetVoice implements Instrument {
  AdditiveEngine engine;
  ADSR adsrEnv;

  MoogFilter lpf;
  ADSR filterEnv;
  Constant filterTarget;

  Noise highBreath;
  BandPass bpf;
  Summer preAdsrMix;   

  TrumpetVoice(float freqHz, float maxAmp, Waveform wf) {
    engine = new AdditiveEngine(freqHz, wf);

    // ==========================================
    // 【ローパスフィルタ】
    // ==========================================
    filterTarget = new Constant(7000.0f);                 
    filterEnv = new ADSR(1.0f, 0.05f, 0.15f, 0.5f, 0.15f);
    lpf = new MoogFilter(0.0f, 0.0f, MoogFilter.Type.LP);
    filterTarget.patch(filterEnv).patch(lpf.frequency);
    engine.getOutput().patch(lpf);                       

    // ==========================================
    // 【10000Hz以上のバンドパス】
    // ==========================================
    highBreath = new Noise(0.001f, Noise.Tint.WHITE);     
    Summer highMix = new Summer();
    engine.wave.patch(highMix);                          
    highBreath.patch(highMix);
    bpf = new BandPass(12500.0f, 3000.0f, 44100.0f);
    highMix.patch(bpf);                                  

    // ==========================================
    // 【合成】
    // ==========================================
    preAdsrMix = new Summer();
    lpf.patch(preAdsrMix);
    bpf.patch(preAdsrMix);

    // 150Hz以下の不要な超低音をカット
    HighPassSP lowCutHpf = new HighPassSP(150.0f, 44100.0f);
    preAdsrMix.patch(lowCutHpf);

    // 音量ADSR→ 低音カット後に適用
    adsrEnv = new ADSR(maxAmp, TRUMPET_ATK_TIME, TRUMPET_DEC_TIME, TRUMPET_SUS_LVL, TRUMPET_REL_TIME);
    lowCutHpf.patch(adsrEnv);
  }

  void noteOn(float duration) {
    adsrEnv.patch(out);
    adsrEnv.noteOn();
    filterEnv.noteOn();   // フィルターのADSRスタート
  }

  void noteOff() {
    adsrEnv.noteOff();
    filterEnv.noteOff();  // フィルターのADSRストップ
    adsrEnv.unpatchAfterRelease(out);
  }
}
