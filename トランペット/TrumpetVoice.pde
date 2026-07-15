class TrumpetVoice implements Instrument {
  AdditiveEngine engine;
  ADSR adsrEnv;
  
  // フィルターを動かすための部品
  MoogFilter lpf;
  ADSR filterEnv;
  Constant filterTarget;

  TrumpetVoice(float freqHz, float maxAmp, Waveform wf) {
    engine = new AdditiveEngine(freqHz, wf);
    
    // ★ フィルタを動かす設定
    filterTarget = new Constant(8000.0f);
    filterEnv = new ADSR(1.0f, 0.05f, 0.15f, 0.5f, 0.15f);
    lpf = new MoogFilter(0.0f, 0.0f, MoogFilter.Type.LP);
    filterTarget.patch(filterEnv).patch(lpf.frequency);
    
    // 音量の設定（定数ファイルから読み込み）
    adsrEnv = new ADSR(maxAmp, TRUMPET_ATK_TIME, TRUMPET_DEC_TIME, TRUMPET_SUS_LVL, TRUMPET_REL_TIME);
    
    // 接続の順番：波形＋ノイズ(engine) → 動くフィルタ(lpf) → 音量ADSR(adsrEnv)
    engine.getOutput().patch(lpf).patch(adsrEnv);
  }

  void noteOn(float duration) { 
    adsrEnv.patch(out);
    adsrEnv.noteOn(); 
    filterEnv.noteOn(); // フィルターのADSRスタート
  }
  
  void noteOff() { 
    adsrEnv.noteOff(); 
    filterEnv.noteOff(); // フィルターのADSRストップ
    adsrEnv.unpatchAfterRelease(out); 
  }
}
