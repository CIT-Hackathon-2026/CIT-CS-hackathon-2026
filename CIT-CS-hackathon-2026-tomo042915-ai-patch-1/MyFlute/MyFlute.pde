class FluteVoice implements Instrument {
  AdditiveEngine engine;
  ADSR adsr;

  FluteVoice(float freq, float maxAmp, Waveform wf) {
    engine = new AdditiveEngine(freq, wf);

    adsr = new ADSR(maxAmp,
                    FLUTE_ATK, FLUTE_DEC,
                    FLUTE_SUS, FLUTE_REL);

    engine.getOutput().patch(adsr);
  }

  void noteOn(float d) {
    adsr.patch(out);
    adsr.noteOn();
  }

  void noteOff() {
    adsr.noteOff();
    adsr.unpatchAfterRelease(out);
  }
}
