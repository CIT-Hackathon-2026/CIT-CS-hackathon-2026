class SynthVoice implements Instrument {
  AdditiveEngine engine;
  MoogFilter moogFilter;
  ADSR adsrEnv;

  SynthVoice(float freqHz, float maxAmp, Waveform wf) {
    engine = new AdditiveEngine(freqHz, wf);
    
    moogFilter = new MoogFilter(freqHz * SYNTH_FILTER_MULT, SYNTH_FILTER_RES);
    adsrEnv = new ADSR(maxAmp, SYNTH_ATK_TIME, SYNTH_DEC_TIME, SYNTH_SUS_LVL, SYNTH_REL_TIME);
    
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
