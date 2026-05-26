class TrumpetVoice implements Instrument {
  AdditiveEngine engine;
  ADSR adsrEnv;

  TrumpetVoice(float freqHz, float maxAmp, Waveform wf) {
    engine = new AdditiveEngine(freqHz, wf);
    
    adsrEnv = new ADSR(maxAmp, TRUMPET_ATK_TIME, TRUMPET_DEC_TIME, TRUMPET_SUS_LVL, TRUMPET_REL_TIME);
    
    engine.getOutput().patch(adsrEnv);
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
