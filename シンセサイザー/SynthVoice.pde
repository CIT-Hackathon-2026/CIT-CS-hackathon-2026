/**
 * 役割: 鳴らせる楽器の単位
 * 規約: {楽器名}Voice
 */
class SynthVoice implements Instrument {
  AdditiveEngine engine; // エンジンを部品として持つ
  MoogFilter moogFilter;
  ADSR adsrEnv;

  SynthVoice(float freqHz, float maxAmp, Waveform wf) {
    // エンジンの生成
    engine = new AdditiveEngine(freqHz, wf);
    
    // フィルターとエンベロープの生成
    moogFilter = new MoogFilter(freqHz * SYNTH_FILTER_MULT, SYNTH_FILTER_RES);
    adsrEnv = new ADSR(maxAmp, SYNTH_ATK_TIME, SYNTH_DEC_TIME, SYNTH_SUS_LVL, SYNTH_REL_TIME);
    
    // エンジンの出力をフィルター → ADSR の順に繋ぐ
    engine.getOutput().patch(moogFilter).patch(adsrEnv);
  }

  void noteOn(float duration) { 
    adsrEnv.patch(out); 
    adsrEnv.noteOn(); 
  }
  
  void noteOff() { 
    adsrEnv.noteOff(); 
    adsrEnv.unpatchAfterRelease(out); 
  }
}
