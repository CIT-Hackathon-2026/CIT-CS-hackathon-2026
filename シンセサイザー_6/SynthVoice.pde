class SynthVoice implements Instrument {
  AdditiveEngine engine; // エンジンを部品として持つ
  ADSR adsrEnv;          // 音量用ADSR

  SynthVoice(float freqHz, float maxAmp, Waveform wf) {
    // エンジンの生成
    engine = new AdditiveEngine(freqHz, maxAmp, wf);

    // 音量エンベロープの生成
    adsrEnv = new ADSR(maxAmp, SYNTH_ATK_TIME, SYNTH_DEC_TIME, SYNTH_SUS_LVL, SYNTH_REL_TIME);

    // エンジンの出力をADSRへ繋ぐ
    engine.getOutput().patch(adsrEnv);
  }

  void noteOn(float duration) {
    adsrEnv.patch(out);
    adsrEnv.noteOn();
    engine.filterEnv.noteOn(); // フィルター専用ADSRも同時に開始
  }

  void noteOff() {
    adsrEnv.noteOff();
    engine.filterEnv.noteOff();
    adsrEnv.unpatchAfterRelease(out);
  }
}
